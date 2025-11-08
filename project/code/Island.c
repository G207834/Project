#include "zf_common_headfile.h"
#include "Island.h"

#define BEEP (C13)

int Island_State4_StartLie = 0;   // 状态4巡线开始列
int Island_Out_Point[2]    = {0}; // 小环出环角点消失位置
int Island_Number          = 0;   // 环岛编号

extern void Left_Add_Line(int x1, int y1, int x2, int y2);
extern void Right_Add_Line(int x1, int y1, int x2, int y2);

extern uint8 mt9v03x_image_TwoValues[MT9V03X_H][MT9V03X_W]; // 二值化后的数组
extern volatile int Longest_WhiteLie_L[2];                  // 左最长白列，[0]白列长度，[1]白列位置，第几列
extern volatile int Longest_WhiteLie_R[2];                  // 右最长白列，[0]白列长度，[1]白列位置，第几列
extern volatile int Search_Stop_Line;                       // 搜索截止行的长度，要得到坐标用高度减去此值
extern volatile int White_Lie[MT9V03X_W][3];                // 0白列长度，1白列起始行数，2白列终止行数
extern volatile int L_Line[MT9V03X_H];                      // 左边线数组
extern volatile int R_Line[MT9V03X_H];                      // 右边线数组
extern volatile int L_Flag[MT9V03X_H];                      // 寻到左边线状态
extern volatile int R_Flag[MT9V03X_H];                      // 寻到左边线状态
extern volatile int R_LostLine_Time;                        // 右边线丢线数
extern volatile int L_LostLine_Time;                        // 左边线丢线数
extern volatile int Both_LostLine_Time;                     // 双边边线丢线数
extern volatile int Boundry_Start_L;                        // 左边第一个边界点，第几行
extern volatile int Boundry_Start_R;                        // 右边第一个边界点，第几行
extern int Road_Wide[MT9V03X_H];                            // 赛道宽度

/*十字角点 */
extern int L_Down_Point;
extern int R_Down_Point;
extern int L_Up_Point;
extern int R_Up_Point;

/* 标志位元素 */
extern int Straight_Flag;       // 直道标志位
extern int Island_State;        // 环岛标志位
extern int Island_Flag_L;       // 环岛标志位
extern int Island_Flag_R;       // 环岛标志位
extern int Ramp_Flag;           // 坡道标志位
extern int Cross_Flag;          // 十字标志位
extern int Zebra_Crossing_Flag; // 斑马线标志位

/* 环岛相关 */
uint8 Continuity_Change_Flag_L = 0; // 连续性断行，连续置零，否则为断行
uint8 Continuity_Change_Flag_R = 0;
int Monotonicity_Change_Line_R = 0;   // 右边界单调性转变行
int Monotonicity_Change_Line_L = 0;   // 左边界单调性转变行
int Right_Down_Line            = 0;   // 右下角点所在行
int Left_Down_Line             = 0;   // 左下角点所在行
int Island_State3_Point[2]     = {0}; //[0]行，[1]列

/* 摄像头误差 */
extern volatile float Err;

extern void Right_Add_Line(int x1, int y1, int x2, int y2);

/// @brief 右下角点检测
/// @param start 起始行
/// @param end 终止行
/// @return 0—未找到角点
int Find_Right_Down_Point(int start, int end) // 右下角点，返回值是角点所在的行数
{
    int i, t;
    Right_Down_Line = 0;
    if (R_LostLine_Time >= 0.9 * MT9V03X_H) // 大部分都丢线，没有拐点判断的意义
        return Right_Down_Line;
    if (start < end) {
        t     = start;
        start = end;
        end   = t;
    }
    if (start >= MT9V03X_H - 1 - 5) // 下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = MT9V03X_H - 1 - 5;
    if (end <= MT9V03X_H - Search_Stop_Line)
        end = MT9V03X_H - Search_Stop_Line;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--) {
        if (Right_Down_Line == 0 &&                // 只找第一个符合条件的点
            abs(R_Line[i] - R_Line[i + 1]) <= 5 && // 角点的阈值可以更改
            abs(R_Line[i + 1] - R_Line[i + 2]) <= 5 &&
            abs(R_Line[i + 2] - R_Line[i + 3]) <= 5 &&
            (R_Line[i] - R_Line[i - 2]) <= -5 &&
            (R_Line[i] - R_Line[i - 3]) <= -10 &&
            (R_Line[i] - R_Line[i - 4]) <= -10) {
            Right_Down_Line = i; // 获取行数即可
            break;
        }
    }
    return Right_Down_Line;
}

/// @brief 左下角点检测
/// @param start 起始行
/// @param end 终止行
/// @return 0—未找到角点
/// @note 从下往上扫
int Find_Left_Down_Point(int start, int end) // 左下角点，返回值是角点所在的行数
{
    int i, t;
    Left_Down_Line = 0;
    if (L_LostLine_Time >= 0.9 * MT9V03X_H) // 大部分都丢线，没有拐点判断的意义
        return Left_Down_Line;
    if (start < end) {
        t     = start;
        start = end;
        end   = t;
    }
    if (start >= MT9V03X_H - 1 - 5) // 下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = MT9V03X_H - 1 - 5;
    if (end <= MT9V03X_H - Search_Stop_Line)
        end = MT9V03X_H - Search_Stop_Line;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--) {
        if (Left_Down_Line == 0 &&                 // 只找第一个符合条件的点
            abs(L_Line[i] - L_Line[i + 1]) <= 5 && // 角点的阈值可以更改
            abs(L_Line[i + 1] - L_Line[i + 2]) <= 8 &&
            abs(L_Line[i + 2] - L_Line[i + 3]) <= 9 &&
            (L_Line[i] - L_Line[i - 2]) >= 5 &&
            (L_Line[i] - L_Line[i - 3]) >= 8 &&
            (L_Line[i] - L_Line[i - 4]) >= 8) {
            Left_Down_Line = i; // 获取行数即可
            break;
        }
    }
    return Left_Down_Line;
}

/// @brief 右环时左右赛道连续性检测
/// @param start_hang  起始行
/// @param end_hang 终止行
///  @note 连续性的阈值设置为5，可更改
void Continuity_Change_R(int start_hang, int end_hang)
{
    int i;
    int t;
    /* 标志位清零 */
    Continuity_Change_Flag_L = 0;
    Continuity_Change_Flag_R = 0;
    /* 删去了丢图判断 */
    if (start_hang >= MT9V03X_H - 5) // 数组越界保护
        start_hang = MT9V03X_H - 5;
    if (end_hang <= 5)
        end_hang = 5;
    if (end_hang <= MT9V03X_H - Search_Stop_Line)
        end_hang = MT9V03X_H - Search_Stop_Line;
    if (start_hang < end_hang) // 都是从下往上计算的，反了就互换一下
    {
        t          = start_hang;
        start_hang = end_hang;
        end_hang   = t;
    }
    for (i = start_hang; i >= end_hang; i--) {
        if (abs(R_Line[i] - R_Line[i - 1]) >= 15) // 连续性阈值是5，可更改
        {
            Continuity_Change_Flag_R = i;
        }
        if (abs(L_Line[i] - L_Line[i - 1]) >= 5) // 连续性阈值是5，可更改
        {
            Continuity_Change_Flag_L = i;
        }
    }
}

/// @brief 左环时左右赛道连续性检测
/// @param start_hang  起始行
/// @param end_hang 终止行
///  @note 连续性的阈值设置为5，可更改
void Continuity_Change_L(int start_hang, int end_hang)
{
    int i;
    int t;
    /* 标志位清零 */
    Continuity_Change_Flag_L = 0;
    Continuity_Change_Flag_R = 0;
    /* 删去了丢图判断 */
    if (start_hang >= MT9V03X_H - 5) // 数组越界保护
    {
        start_hang = MT9V03X_H - 5;
    }
    if (end_hang <= 5) {
        end_hang = 5;
    }

    if (end_hang <= MT9V03X_H - Search_Stop_Line) {
        end_hang = MT9V03X_H - Search_Stop_Line;
    }
    if (start_hang < end_hang) // 都是从下往上计算的，反了就互换一下
    {
        t          = start_hang;
        start_hang = end_hang;
        end_hang   = t;
    }
    for (i = start_hang; i >= end_hang; i--) {
        if (abs(R_Line[i] - R_Line[i - 1]) >= 6) // 连续性阈值是5，可更改
        {
            Continuity_Change_Flag_R = i;
        }
        if (abs(L_Line[i] - L_Line[i - 1]) >= 12) // 连续性阈值是5，可更改
        {
            Continuity_Change_Flag_L = i;
        }
    }
}

/// @brief 右边界单调性突变检测
/// @param start 起始行
/// @param end 终止行
int Monotonicity_Change_Right(int start, int end) // 单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    Monotonicity_Change_Line_R = 0;
    if (R_LostLine_Time >= 0.8 * MT9V03X_H) // 大部分都丢线，没有单调性判断的意义
        return Monotonicity_Change_Line_R;
    if (start >= MT9V03X_H - 1 - 5) // 数组越界保护
        start = MT9V03X_H - 1 - 5;
    if (end <= 5)
        end = 5;
    if (end <= MT9V03X_H - Search_Stop_Line)
        end = MT9V03X_H - Search_Stop_Line;
    if (start <= end) {
        return Monotonicity_Change_Line_R;
    }
    for (i = start; i >= end; i--) // 会读取前5后5数据，所以前面对输入范围有要求
    {
        if (R_Line[i] == R_Line[i + 5] && R_Line[i] == R_Line[i - 5] &&
            R_Line[i] == R_Line[i + 4] && R_Line[i] == R_Line[i - 4] &&
            R_Line[i] == R_Line[i + 3] && R_Line[i] == R_Line[i - 3] &&
            R_Line[i] == R_Line[i + 2] && R_Line[i] == R_Line[i - 2] &&
            R_Line[i] == R_Line[i + 1] && R_Line[i] == R_Line[i - 1]) { // 一堆数据一样，显然不能作为单调转折点
            continue;
        } else if (R_Line[i] <= R_Line[i + 5] && R_Line[i] <= R_Line[i - 5] &&
                   R_Line[i] <= R_Line[i + 4] && R_Line[i] <= R_Line[i - 4] &&
                   R_Line[i] <= R_Line[i + 3] && R_Line[i] <= R_Line[i - 3] &&
                   R_Line[i] <= R_Line[i + 2] && R_Line[i] <= R_Line[i - 2] &&
                   R_Line[i] <= R_Line[i + 1] && R_Line[i] <= R_Line[i - 1]) { // 这个数据是在前5，后5中最小的，那就是单调突变点
            Monotonicity_Change_Line_R = i;
            break;
        }
    }
}

/// @brief 左边界单调性突变检测
/// @param start 起始行
/// @param end 终止行
int Monotonicity_Change_Left(int start, int end) // 单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    Monotonicity_Change_Line_L = 0;
    if (L_LostLine_Time >= 0.8 * MT9V03X_H) // 大部分都丢线，没有单调性判断的意义
        return Monotonicity_Change_Line_L;
    if (start >= MT9V03X_H - 1 - 5) // 数组越界保护
        start = MT9V03X_H - 1 - 5;
    if (end <= 5)
        end = 5;
    if (end <= MT9V03X_H - Search_Stop_Line)
        end = MT9V03X_H - Search_Stop_Line + 1;
    if (start <= end)
        return Monotonicity_Change_Line_L;
    for (i = start; i >= end; i--) // 会读取前5后5数据，所以前面对输入范围有要求
    {
        if (L_Line[i] == L_Line[i + 5] && L_Line[i] == L_Line[i - 5] &&
            L_Line[i] == L_Line[i + 4] && L_Line[i] == L_Line[i - 4] &&
            L_Line[i] == L_Line[i + 3] && L_Line[i] == L_Line[i - 3] &&
            L_Line[i] == L_Line[i + 2] && L_Line[i] == L_Line[i - 2] &&
            L_Line[i] == L_Line[i + 1] && L_Line[i] == L_Line[i - 1]) { // 一堆数据一样，显然不能作为单调转折点
            continue;
        } else if (L_Line[i] >= L_Line[i + 5] && L_Line[i] >= L_Line[i - 5] &&
                   L_Line[i] >= L_Line[i + 4] && L_Line[i] >= L_Line[i - 4] &&
                   L_Line[i] >= L_Line[i + 3] && L_Line[i] >= L_Line[i - 3] &&
                   L_Line[i] >= L_Line[i + 2] && L_Line[i] >= L_Line[i - 2] &&
                   L_Line[i] >= L_Line[i + 1] && L_Line[i] >= L_Line[i - 1]) { // 就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            Monotonicity_Change_Line_L = i;
            break;
        }
    }
    return Monotonicity_Change_Line_L;
}

/// @brief 根据左边界斜率补右边线
/// @param start_line 起始行
/// @param end_Line 终止行
/// @param k 斜率
void Island_Add_Line_R(int start_line, int end_Line, float k) // 补线的起始和截止行
{
    k = -k;
    int hang, t;
    if (start_line > end_Line) //++访问，确保起始行在上方
    {
        t          = start_line;
        start_line = end_Line;
        end_Line   = t;
    }
    for (hang = start_line; hang <= end_Line; hang++) {
        R_Line[hang] = (int)R_Line[start_line] + k * (hang - start_line);
    }
}

/// @brief 根据右边界斜率补左边线
/// @param start_line 起始行
/// @param end_Line 终止行
/// @param k 斜率
void Island_Add_Line_L(int start_line, int end_Line, float k) // 补线的起始和截止行
{
    k = -k;
    int hang, t;
    if (start_line > end_Line) //++访问，确保起始行在上方
    {
        t          = start_line;
        start_line = end_Line;
        end_Line   = t;
    }
    for (hang = start_line; hang <= end_Line; hang++) {
        L_Line[hang] = (int)L_Line[start_line] + k * (hang - start_line);
    }
}

/// @brief 获取左边界某行附近的斜率
/// @param start_line 起始行
/// @return 斜率
float L_Line_K(int start_line)
{
    int end_line = start_line + 5;                                       // 从起始行向下找五行
    float k      = (float)(L_Line[end_line] - L_Line[start_line]) / 5.0; // 求斜率
    return k;
}

/// @brief 获取右边界某行附近的斜率
/// @param start_line 起始行
/// @return 斜率
float R_Line_K(int start_line)
{
    int end_line = start_line + 5;                                       // 从起始行向下找五行
    float k      = (float)(R_Line[end_line] - R_Line[start_line]) / 5.0; // 求斜率
    return k;
}

/// @brief 延长右边界
/// @param start 起始行
/// @param end 终止行
void Island_Lengthen_Right_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= MT9V03X_H - 1) // 起始点位置校正，排除数组越界的可能
        start = MT9V03X_H - 1;
    else if (start <= 0)
        start = 0;
    if (end >= MT9V03X_H - 1)
        end = MT9V03X_H - 1;
    else if (end <= 0)
        end = 0;
    if (end < start) //++访问，坐标互换
    {
        t     = end;
        end   = start;
        start = t;
    }

    if (start <= 5) // 因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(R_Line[start], start, R_Line[end], end);
    } else {
        k = (float)(R_Line[start] - R_Line[start - 5]) / 6.0; // 这里的k是1/斜率
        for (i = start; i <= end; i++) {
            R_Line[i] = (int)(i - start) * k + R_Line[start]; //(x=(y-y1)*k+x1),点斜式变形
            if (R_Line[i] >= MT9V03X_W - 1) {
                R_Line[i] = MT9V03X_W - 1;
            } else if (R_Line[i] <= 0) {
                R_Line[i] = 0;
            }
        }
    }
}

/// @brief 延长左边界
/// @param start 起始行
/// @param end 终止行
void Island_Lengthen_Left_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= MT9V03X_H - 1) // 起始点位置校正，排除数组越界的可能
        start = MT9V03X_H - 1;
    else if (start <= 0)
        start = 0;
    if (end >= MT9V03X_H - 1)
        end = MT9V03X_H - 1;
    else if (end <= 0)
        end = 0;
    if (end < start) //++访问，坐标互换
    {
        t     = end;
        end   = start;
        start = t;
    }

    if (start <= 5) // 因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Left_Add_Line(L_Line[start], start, L_Line[end], end);
    } else {
        k = (float)(L_Line[start] - L_Line[start - 5]) / 6.0; // 这里的k是1/斜率
        for (i = start; i <= end; i++) {
            L_Line[i] = (int)(i - start) * k + L_Line[start]; //(x=(y-y1)*k+x1),点斜式变形
            if (L_Line[i] >= MT9V03X_W - 1) {
                L_Line[i] = MT9V03X_W - 1;
            } else if (L_Line[i] <= 0) {
                L_Line[i] = 0;
            }
        }
    }
}

/// @brief 环岛状态3巡线函数
void Island_State_3_Search_Line()
{
    int hang  = 0; // 行
    int lie   = 0; // 列
    int Count = 0;
    /* 从左到右，从下到上遍历数组，记录每列白点数 */
    /* 初始化整张图的每一列，避免未赋值列导致后续判断异常 */
    for (lie = 0; lie < MT9V03X_W; lie++) {
        White_Lie[lie][0] = 0;
        White_Lie[lie][1] = 0;
        White_Lie[lie][2] = 0;
        Count             = 0;
        for (hang = MT9V03X_H - 1; hang >= 20; hang--) {
            if (mt9v03x_image_TwoValues[hang][lie] == 0) // 在某列找到黑点
            {
                if (White_Lie[lie][1] != 0) // 起始点已记录
                {
                    Count++;
                    if (Count >= 2) {
                        White_Lie[lie][2] = hang + 1;
                    }
                }
            } else // 找到的是白点
            {
                Count = 0;
                if (White_Lie[lie][1] == 0) // 起始点未记录
                {
                    White_Lie[lie][1] = hang;
                }
                if (hang == 0 && White_Lie[lie][2] == 0) // 最上一行，且是白的
                {
                    White_Lie[lie][2] = 0;
                }
            }
            if (White_Lie[lie][1] != 0 && White_Lie[lie][2] != 0) {
                White_Lie[lie][0] = White_Lie[lie][1] - White_Lie[lie][2] + 1;
                break;
            }
        }
    }

    if (Island_Flag_R) {
        /* 找右找角点 */
        Island_State3_Point[0] = 0;
        Island_State3_Point[1] = 0;
        /* 只在可以安全访问 lie-5..lie+4 范围的列中查找角点 */
        /* 离角点远的时候右边基本是直的，改变右边的巡线阈值，可以提升远端的角点判断 */
        for (lie = 10; lie <= MT9V03X_W - 10; lie++) {
            /* if(White_Lie[lie][2] > 30 && White_Lie[lie][0] >= 20 &&
                White_Lie[lie][2] - White_Lie[lie - 5][2] >= 4 &&
                White_Lie[lie][2] - White_Lie[lie - 6][2] >= 5 &&
                White_Lie[lie][2] - White_Lie[lie - 8][2] >= 6 &&
                White_Lie[lie][2] - White_Lie[lie + 7][2] >= 1 &&
                White_Lie[lie][2] - White_Lie[lie + 9][2] >= 1 &&
                White_Lie[lie][2] - White_Lie[lie + 10][2] >= 2) {
                Island_State3_Point[0] = White_Lie[lie][2];
                Island_State3_Point[1] = lie;
                break;
            } */
            if (White_Lie[lie][2] > 20 && White_Lie[lie][0] >= 20 &&
                White_Lie[lie][2] - White_Lie[lie - 5][2] >= 3 &&
                White_Lie[lie][2] - White_Lie[lie - 6][2] >= 4 &&
                White_Lie[lie][2] - White_Lie[lie - 8][2] >= 6 &&
                White_Lie[lie][2] - White_Lie[lie + 6][2] >= -2 &&
                White_Lie[lie][2] - White_Lie[lie + 7][2] >= -2 &&
                White_Lie[lie][2] - White_Lie[lie + 9][2] >= -2) {
                Island_State3_Point[0] = White_Lie[lie][2];
                Island_State3_Point[1] = lie;
                /* Point_Flag= Island_State3_Point[0];
                Point_Flag1=Island_State3_Point[1]; */
                break;
            }
        }

        /* 从右角点所在列到右找右边最长白列 */
        Longest_WhiteLie_R[0] = 0;
        Longest_WhiteLie_R[1] = 0;
        if (Island_State3_Point[1] != 0) {
            for (lie = Island_State3_Point[1] + 10; lie <= MT9V03X_W - 10; lie++) {
                if (White_Lie[lie][0] >= Longest_WhiteLie_R[0]) {
                    Longest_WhiteLie_R[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_R[1] = lie;               // 记录下标
                    Longest_WhiteLie_L[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_L[1] = lie;               // 记录下标
                }
            }
        } else {
            for (lie = 35; lie <= MT9V03X_W - 10; lie++) {
                if (White_Lie[lie][0] >= Longest_WhiteLie_R[0]) {
                    Longest_WhiteLie_R[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_R[1] = lie;               // 记录下标
                    Longest_WhiteLie_L[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_L[1] = lie;               // 记录下标
                }
            }
        }
    } else {
        /* 找左找角点 */
        Island_State3_Point[0] = 0;
        Island_State3_Point[1] = 0;
        /* 只在可以安全访问 lie-5..lie+4 范围的列中查找角点 */
        /* 离角点远的时候右边基本是直的，改变右边的巡线阈值，可以提升远端的角点判断 */
        for (lie = MT9V03X_W - 10; lie >= 10; lie--) {
            /* if (White_Lie[lie][2] > 30 && White_Lie[lie][0] >= 20 &&
                White_Lie[lie][2] - White_Lie[lie - 5][2] >= 4 &&
                White_Lie[lie][2] - White_Lie[lie - 6][2] >= 5 &&
                White_Lie[lie][2] - White_Lie[lie - 8][2] >= 6 &&
                White_Lie[lie][2] - White_Lie[lie + 7][2] >= 1 &&
                White_Lie[lie][2] - White_Lie[lie + 9][2] >= 1 &&
                White_Lie[lie][2] - White_Lie[lie + 10][2] >= 2) {
                Island_State3_Point[0] = White_Lie[lie][2];
                Island_State3_Point[1] = lie;
                break;
            } */
            if (White_Lie[lie][2] > 20 && White_Lie[lie][0] >= 20 &&
                White_Lie[lie][2] - White_Lie[lie + 5][2] >= 3 &&
                White_Lie[lie][2] - White_Lie[lie + 6][2] >= 4 &&
                White_Lie[lie][2] - White_Lie[lie + 8][2] >= 6 &&
                White_Lie[lie][2] - White_Lie[lie - 6][2] >= -2 &&
                White_Lie[lie][2] - White_Lie[lie - 7][2] >= -2 &&
                White_Lie[lie][2] - White_Lie[lie - 9][2] >= -2) {
                Island_State3_Point[0] = White_Lie[lie][2];
                Island_State3_Point[1] = lie;
                /* Point_Flag             = Island_State3_Point[0];
                Point_Flag1            = Island_State3_Point[1]; */
                break;
            }
        }

        /* 从左角点所在列到左找左边最长白列 */
        Longest_WhiteLie_R[0] = 0;
        Longest_WhiteLie_R[1] = 0;
        if (Island_State3_Point[1] != 0) {
            for (lie = Island_State3_Point[1] - 10; lie >= 10; lie--) {
                if (White_Lie[lie][0] >= Longest_WhiteLie_R[0]) {
                    Longest_WhiteLie_R[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_R[1] = lie;               // 记录下标
                    Longest_WhiteLie_L[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_L[1] = lie;               // 记录下标
                }
            }
        } else {
            for (lie = MT9V03X_W - 1 - 35; lie >= 10; lie--) {
                if (White_Lie[lie][0] >= Longest_WhiteLie_R[0]) {
                    Longest_WhiteLie_R[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_R[1] = lie;               // 记录下标
                    Longest_WhiteLie_L[0] = White_Lie[lie][0]; // 记录长度
                    Longest_WhiteLie_L[1] = lie;               // 记录下标
                }
            }
        }
    }

    /* 搜索截止行设为最长白列长度 */
    Search_Stop_Line = Longest_WhiteLie_R[0];

    for (hang = MT9V03X_H - 1; hang >= MT9V03X_H - Search_Stop_Line; hang--) {
        /* 从右最长白列向左寻线 */
        for (lie = Longest_WhiteLie_R[1]; lie >= 0 + 2; lie--) {
            if (mt9v03x_image_TwoValues[hang][lie] == 255 && mt9v03x_image_TwoValues[hang][lie - 1] == 0 && mt9v03x_image_TwoValues[hang][lie - 2] == 0) {
                L_Line[hang] = lie;
                L_Flag[hang] = 1;
                break;
            } else if (lie <= 2) // 没找到，则把图像边界当做边线
            {
                L_Line[hang] = 0;
                L_Flag[hang] = 0;
                break;
            }
        }
        /* 从右最长白列向右寻线 */
        for (lie = Longest_WhiteLie_R[1]; lie <= MT9V03X_W - 1 - 2; lie++) {
            if (mt9v03x_image_TwoValues[hang][lie] == 255 && mt9v03x_image_TwoValues[hang][lie + 1] == 0 && mt9v03x_image_TwoValues[hang][lie + 2] == 0) {
                R_Line[hang] = lie;
                R_Flag[hang] = 1;
                break;
            } else if (lie >= MT9V03X_W - 1 - 2) // 没找到，则把图像边界当做边线
            {
                R_Line[hang] = MT9V03X_W;
                R_Flag[hang] = 0;
                break;
            }
        }
    }
    /* ips200_show_int(200, 210, Point_Flag, 3);
    ips200_show_int(240, 210, Point_Flag1, 3); */
}

/// @brief 环岛判断
void Image_Island_Dect()
{
    int Island_3to4_Count = 0;
    if (Cross_Flag == 0) {
        int lie = 0;
        if (Island_State == 3) {
            Island_State_3_Search_Line();
        }
        if (Island_State == 0) {
            if (Island_Flag_R == 0) {
                Continuity_Change_R(MT9V03X_H - 10, 50);                                                                               // 判断赛道连续性
                if (Continuity_Change_Flag_R != 0 && Continuity_Change_Flag_L == 0 && Both_LostLine_Time < 5 && Search_Stop_Line > 99) // 如果满足右边线撕裂，左边线连续，丢线数小于阈值，视野足够远
                {
                    Find_Right_Down_Point(MT9V03X_H - 1, 50); // 找右下角点
                    /* 判定右环 */
                    if (R_Line[Right_Down_Line] >= MT9V03X_W - 50 && Right_Down_Line >= MT9V03X_H - 40 && L_LostLine_Time <= 6 && Road_Wide[Right_Down_Line] >= 30) // 限定角点出现的位置
                    {
                        Island_Flag_R = 1; // 判断进入环岛
                        Island_State  = 1;
                        gpio_set_level(BEEP, GPIO_HIGH);
                    } else {
                        Island_Flag_R = 0;
                        Island_State  = 0;
                    }
                }
            }
            if (Island_Flag_L == 0) {
                Continuity_Change_L(MT9V03X_H - 10, 50);
                if (Continuity_Change_Flag_L != 0 && Continuity_Change_Flag_R == 0 && Both_LostLine_Time < 8 && Search_Stop_Line > 99) // 如果满足右边线撕裂，左边线连续，丢线数小于阈值，视野足够远
                {
                    Find_Left_Down_Point(MT9V03X_H - 1, 50); // 找左下角点
                    /* 判定左环 */
                    if (L_Line[Left_Down_Line] <= 55 && Left_Down_Line >= MT9V03X_H - 45 && R_LostLine_Time <= 8 && Road_Wide[Left_Down_Line] >= 23) // 限定角点出现的位置
                    {
                        Island_Flag_L = 1; // 判断进入环岛
                        Island_State  = 1;
                        gpio_set_level(BEEP, GPIO_HIGH);
                    } else {
                        Island_Flag_L = 0;
                        Island_State  = 0;
                    }
                }
            }
        } else if (Island_State == 1) // 判断进入环岛后,右下方边界未消失
        {
            if (Island_Flag_R == 1) // 右环
            {
                Monotonicity_Change_Right(80, 10);
                if (Monotonicity_Change_Line_R != 0) {
                    /* 下面是连点补线 */
                    // Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R,R_Line[R_Down_Point],R_Down_Point);
                    /* 下面是左边界对称补线 */
                    Island_Add_Line_R(Monotonicity_Change_Line_R, MT9V03X_H - 1, L_Line_K(Monotonicity_Change_Line_R)); // 直接拉到最底部
                }
                /* 下方边界消失,进入状态2 */
                if (Boundry_Start_R < MT9V03X_H - 20) {
                    Island_State = 2;
                    gpio_set_level(BEEP, GPIO_LOW);
                }
            } else {
                Monotonicity_Change_Left(80, 10);
                if (Monotonicity_Change_Line_L != 0) {
                    /* 下面是右边界对称补线 */
                    Island_Add_Line_L(Monotonicity_Change_Line_L, MT9V03X_H - 1, R_Line_K(Monotonicity_Change_Line_L)); // 直接拉到最底部
                }
                /* 下方边界消失,进入状态2 */
                if (Boundry_Start_L < MT9V03X_H - 20) {
                    Island_State = 2;
                    gpio_set_level(BEEP, GPIO_LOW);
                }
            }
        } else if (Island_State == 2) // 判断进入环岛后,右下方边界消失
        {
            if (Island_Flag_R) {
                Monotonicity_Change_Right(MT9V03X_H - 8, 30);
                if (Monotonicity_Change_Line_R != 0) {
                    /* 下面是连点补线 */
                    // Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R,R_Line[R_Down_Point],R_Down_Point);
                    /* 下面是左边界对称补线 */
                    Island_Add_Line_R(Monotonicity_Change_Line_R, MT9V03X_H - 1, L_Line_K(Monotonicity_Change_Line_R)); // 直接拉线到最底部
                }
                if (Boundry_Start_R >= MT9V03X_H - 5 || Monotonicity_Change_Line_R > 75) // 当圆弧靠下或者下方不丢线
                {
                    Island_State = 3;
                    gpio_set_level(BEEP, GPIO_HIGH);
                }
            } else {
                Monotonicity_Change_Left(MT9V03X_H - 8, 30);
                if (Monotonicity_Change_Line_L != 0) {
                    Island_Add_Line_L(Monotonicity_Change_Line_L, MT9V03X_H - 1, R_Line_K(Monotonicity_Change_Line_L)); // 直接拉到最底部
                }
                if (Boundry_Start_L >= MT9V03X_H - 5 || Monotonicity_Change_Line_L > 75) // 当圆弧靠下或者下方不丢线
                {
                    Island_State = 3;
                    gpio_set_level(BEEP, GPIO_HIGH);
                }
            }
        } else if (Island_State == 3) {
            if (Island_Flag_R) {
                /* 仅在找到有效的角点时才补线，避免使用未初始化或无效点 */
                if (Island_State3_Point[0] != 0 && Island_State3_Point[1] != 0) {
                    /* 调用 Left_Add_Line(x1=col, y1=row, x2=col2, y2=row2)
                       这里将右下角点与图像底部（同列）连线作为左边界近似：
                       将第二点设置为同列、底部行（MT9V03X_H-1）以延展到图像底部 */
                    Left_Add_Line(Island_State3_Point[1], Island_State3_Point[0], L_Line[119], MT9V03X_H - 1);
                    // Left_Add_Line(Island_State3_Point[1], Island_State3_Point[0], L_Line[110], MT9V03X_H - 11);
                }
                if ((Island_State3_Point[0] >= MT9V03X_H - 20 || Island_State3_Point[1] <= 23) && Island_State3_Point[1] != 0) {
                    Island_State           = 4;
                    Island_State4_StartLie = Island_State3_Point[1];
                    gpio_set_level(BEEP, GPIO_LOW);
                } else {
                    if (Island_State3_Point[0] == 0 && Island_State3_Point[1] == 0) {
                        if (Boundry_Start_L > MT9V03X_H - 18 && Search_Stop_Line <= MT9V03X_H - 23) {
                            for (lie = 10; lie <= 55; lie++) {
                                if (White_Lie[lie][2] >= 30 && White_Lie[lie][1] >= MT9V03X_H - 8 && (White_Lie[lie][2] - White_Lie[lie + 3][2] >= 0 && White_Lie[lie][2] - White_Lie[lie + 10][2] < 25)) {
                                    Island_3to4_Count++;
                                }
                            }
                            if (Island_3to4_Count >= 15) {
                                Island_State           = 4;
                                Island_State4_StartLie = 70;
                                gpio_set_level(BEEP, GPIO_LOW);
                            }
                        }
                    }
                }
            } else {
                if (Island_State3_Point[0] != 0 && Island_State3_Point[1] != 0) {
                    /* 调用 Left_Add_Line(x1=col, y1=row, x2=col2, y2=row2)
                       这里将右下角点与图像底部（同列）连线作为左边界近似：
                       将第二点设置为同列、底部行（MT9V03X_H-1）以延展到图像底部 */
                    Right_Add_Line(Island_State3_Point[1], Island_State3_Point[0], R_Line[119], MT9V03X_H - 1);
                    // Left_Add_Line(Island_State3_Point[1], Island_State3_Point[0], L_Line[110], MT9V03X_H - 11);
                }
                if ((Island_State3_Point[0] >= MT9V03X_H - 20 || Island_State3_Point[1] >= MT9V03X_W - 1 - 23) && Island_State3_Point[1] != 0) {
                    Island_State           = 4;
                    Island_State4_StartLie = Island_State3_Point[1];
                    gpio_set_level(BEEP, GPIO_LOW);
                } else {
                    if (Island_State3_Point[0] == 0 && Island_State3_Point[1] == 0) {
                        if (Boundry_Start_R > MT9V03X_H - 18 && Search_Stop_Line <= MT9V03X_H - 23) {
                            for (lie = MT9V03X_W - 1 - 10; lie >= MT9V03X_W - 1 - 55; lie--) {
                                if (White_Lie[lie][2] >= 30 && White_Lie[lie][1] >= MT9V03X_H - 8 && (White_Lie[lie][2] - White_Lie[lie - 3][2] >= 0 && White_Lie[lie][2] - White_Lie[lie - 10][2] < 25)) {
                                    Island_3to4_Count++;
                                }
                            }
                            if (Island_3to4_Count >= 15) {
                                Island_State           = 4;
                                Island_State4_StartLie = 70;
                                gpio_set_level(BEEP, GPIO_LOW);
                            }
                        }
                    }
                }
            }

        } else if (Island_State == 4) {
            Island_State = 5;
            /* if(陀螺仪)
            {
                Island_State = 5;
            } */
        } else if (Island_State == 5) {
            if (Island_Flag_R) {
                Monotonicity_Change_Left(MT9V03X_H - 10, 40);
                /* ips200_show_int(200, 210, L_Line[Monotonicity_Change_Line_L], 3);
                ips200_show_int(240, 210, Monotonicity_Change_Line_L, 3); */
                /* Left_Add_Line(L_Line[Monotonicity_Change_Line_L], Monotonicity_Change_Line_L, 141, (White_Lie[MT9V03X_W - 1][2] + White_Lie[141][2]) / 2); */
                // if (Monotonicity_Change_Line_L != 0 && Both_LostLine_Time > 8) {
                if (Monotonicity_Change_Line_L != 0) {
                    Left_Add_Line(L_Line[Monotonicity_Change_Line_L], Monotonicity_Change_Line_L, MT9V03X_W - 1, 0);
                    if ((Monotonicity_Change_Line_L >= MT9V03X_H - 20 || L_Line[Monotonicity_Change_Line_L] <= 20) && Road_Wide[Monotonicity_Change_Line_L] >= 2) {
                        Island_State = 6;
                        gpio_set_level(BEEP, GPIO_HIGH);
                        if (Island_Number == 1) {
                            Island_Out_Point[0] = Monotonicity_Change_Line_L;
                            Island_Out_Point[1] = L_Line[Monotonicity_Change_Line_L];
                        }
                    }
                }
            } else {
                Monotonicity_Change_Right(MT9V03X_H - 10, 60);
                /* ips200_show_int(200, 210, L_Line[Monotonicity_Change_Line_L], 3);
                ips200_show_int(240, 210, Monotonicity_Change_Line_L, 3); */
                /* Left_Add_Line(L_Line[Monotonicity_Change_Line_L], Monotonicity_Change_Line_L, 141, (White_Lie[MT9V03X_W - 1][2] + White_Lie[141][2]) / 2); */
                // if (Monotonicity_Change_Line_L != 0 && Both_LostLine_Time > 8) {
                if (Monotonicity_Change_Line_R != 0) {
                    Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R, 0, 0);
                    if ((Monotonicity_Change_Line_R >= MT9V03X_H - 25 || R_Line[Monotonicity_Change_Line_R] >= MT9V03X_W-25) && Road_Wide[Monotonicity_Change_Line_R] >= 2) {
                        Island_State = 6;
                        gpio_set_level(BEEP, GPIO_HIGH);
                        if (Island_Number == 1) {
                            Island_Out_Point[0] = Monotonicity_Change_Line_R;
                            Island_Out_Point[1] = R_Line[Monotonicity_Change_Line_R];
                        }
                    }
                }
            }

        } else if (Island_State == 6) {
            if (Island_Flag_R) {
                /* 找右找角点 */
                Island_State3_Point[0] = 0;
                Island_State3_Point[1] = 0;
                /* 只在可以安全访问 lie-5..lie+4 范围的列中查找角点 */
                /* 离角点远的时候右边基本是直的，改变右边的巡线阈值，可以提升远端的角点判断 */
                for (lie = 40; lie <= MT9V03X_W - 11; lie++) {
                    if (White_Lie[lie][2] > 20 && White_Lie[lie][0] >= 20 &&
                        White_Lie[lie][2] - White_Lie[lie - 9][2] >= 1 &&
                        White_Lie[lie][2] - White_Lie[lie - 10][2] >= 2 &&
                        White_Lie[lie][2] - White_Lie[lie - 11][2] >= 3 &&
                        White_Lie[lie][2] - White_Lie[lie + 7][2] >= -2 &&
                        White_Lie[lie][2] - White_Lie[lie + 9][2] >= -2 &&
                        White_Lie[lie][2] - White_Lie[lie + 10][2] >= -2) {
                        Island_State3_Point[0] = White_Lie[lie][2];
                        Island_State3_Point[1] = lie;
                        break;
                    }
                    /* Monotonicity_Change_Right(70, 5);
                    if (Monotonicity_Change_Line_R != 0) {
                        Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R, R_Line[R_Down_Point], R_Down_Point);
                    } */
                    /* Island_Add_Line_R(Island_State3_Point[0], MT9V03X_H, L_Line_K(Island_State3_Point[0])); // 直接拉线到最底部 */
                    /* 补线 */
                }
                if (Island_State3_Point[0] != 0) {
                    Island_Lengthen_Right_Boundry(Island_State3_Point[0], MT9V03X_H - 3);
                } else {
                    if (Island_Number == 1) {
                        Left_Add_Line(94, White_Lie[94][2], Island_Out_Point[1], Island_Out_Point[0]);
                    }
                }
                if (Island_State3_Point[0] > MT9V03X_H - 33) {
                    gpio_set_level(BEEP, GPIO_LOW);
                    Island_State  = 0;
                    Island_Flag_R = 0;
                    Island_Number++;
                }
            } else {
                /* 找左找角点 */
                Island_State3_Point[0] = 0;
                Island_State3_Point[1] = 0;
                /* 只在可以安全访问 lie-5..lie+4 范围的列中查找角点 */
                /* 离角点远的时候右边基本是直的，改变右边的巡线阈值，可以提升远端的角点判断 */
                for (lie = MT9V03X_W - 40; lie >= 11; lie--) {
                    if (White_Lie[lie][2] > 30 && White_Lie[lie][0] >= 30 &&
                        White_Lie[lie][2] - White_Lie[lie + 9][2] >= 4 &&
                        White_Lie[lie][2] - White_Lie[lie + 10][2] >= 5 &&
                        White_Lie[lie][2] - White_Lie[lie + 11][2] >= 6 &&
                        White_Lie[lie][2] - White_Lie[lie - 7][2] >= -2 &&
                        White_Lie[lie][2] - White_Lie[lie - 9][2] >= -2 &&
                        White_Lie[lie][2] - White_Lie[lie - 10][2] >= -2) {
                        Island_State3_Point[0] = White_Lie[lie][2];
                        Island_State3_Point[1] = lie;
                        break;
                    }
                    /* Monotonicity_Change_Right(70, 5);
                    if (Monotonicity_Change_Line_R != 0) {
                        Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R, R_Line[R_Down_Point], R_Down_Point);
                    } */
                    /* Island_Add_Line_R(Island_State3_Point[0], MT9V03X_H, L_Line_K(Island_State3_Point[0])); // 直接拉线到最底部 */
                    /* 补线 */
                }
                if (Island_State3_Point[0] != 0) {
                    Island_Lengthen_Left_Boundry(Island_State3_Point[0], MT9V03X_H - 3);
                } else {
                    if (Island_Number == 1) {
                        Right_Add_Line(94, White_Lie[94][2], Island_Out_Point[1], Island_Out_Point[0]);
                    }
                }
                if (Island_State3_Point[0] > MT9V03X_H - 33) {
                    gpio_set_level(BEEP, GPIO_LOW);
                    Island_State  = 0;
                    Island_Flag_L = 0;
                    Island_Number++;
                }
            }
        }
    }
}
