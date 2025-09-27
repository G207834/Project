#include "zf_common_headfile.h"
#include "Island.h"

extern void Left_Add_Line(int x1, int y1, int x2, int y2);

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

extern int Road_Wide[MT9V03X_H]; // 赛道宽度

/*十字角点 */
extern int L_Down_Point;
extern int R_Down_Point;
extern int L_Up_Point;
extern int R_Up_Point;

extern volatile float Err; // 摄像头误差

/* 标志位元素 */
extern int Straight_Flag; // 直道标志位
extern int Island_State;  // 环岛标志位
extern int Island_Flag_L; // 环岛标志位
extern int Island_Flag_R; // 环岛标志位
extern int Ramp_Flag;     // 坡道标志位
extern int Cross_Flag;    // 十字标志位

/* 环岛相关 */
uint8 Continuity_Change_Flag_L = 0; // 连续性断行，连续置零，否则为断行
uint8 Continuity_Change_Flag_R = 0;
int Monotonicity_Change_Line_R = 0;   // 单调性转变行
int Right_Down_Line            = 0;   // 右下角点所在行
int Island_State3_Point[2]     = {0}; //[0]行，[1]列

extern void Right_Add_Line(int x1, int y1, int x2, int y2);

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start, int end) // 找四个角点，返回值是角点所在的行数
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
    return 0;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左右赛道连续性检测
  @param     起始行，终止行
  @return    连续返回0，不连续返回断线出行数
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
void Continuity_Change(int start_hang, int end_hang)
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
    if (start_hang < end_hang) // 都是从下往上计算的，反了就互换一下
    {
        t          = start_hang;
        start_hang = end_hang;
        end_hang   = t;
    }
    for (i = start_hang; i >= end_hang; i--) {
        if (abs(R_Line[i] - R_Line[i - 1]) >= 5) // 连续性阈值是5，可更改
        {
            Continuity_Change_Flag_R = i;
        }
        if (abs(L_Line[i] - L_Line[i - 1]) >= 5) // 连续性阈值是5，可更改
        {
            Continuity_Change_Flag_L = i;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start, int end) // 单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    Monotonicity_Change_Line_R = 0;
    if (R_LostLine_Time >= 0.9 * MT9V03X_H) // 大部分都丢线，没有单调性判断的意义
        return Monotonicity_Change_Line_R;
    if (start >= MT9V03X_H - 1 - 5) // 数组越界保护
        start = MT9V03X_H - 1 - 5;
    if (end <= 5)
        end = 5;
    if (start <= end)
        return Monotonicity_Change_Line_R;
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
                   R_Line[i] <= R_Line[i + 1] && R_Line[i] <= R_Line[i - 1]) { // 就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            Monotonicity_Change_Line_R = i;
            break;
        }
    }
    return Monotonicity_Change_Line_R;
}

void Island_Add_Line_R(int start_line, int end_Line, float k) // 补线的起始和截止行
{
    k = 1 / k;
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

float L_Line_K(int start_line)
{
    int end_line = start_line + 5;                       // 从起始行向下找五行
    float k      = (float)(end_line - start_line) / 5.0; // 求斜率
    return k;
}

/* 环岛状态3巡线函数 */
void Island_State_3_Search_Line()
{
    int start_lie = 20; // 最长白列搜索区间，图像左右减去此长度
    int end_lie   = MT9V03X_W - 1 - start_lie;
    int hang      = 0; // 行
    int lie       = 0; // 列
    /* 从左到右，从下到上遍历数组，记录每列白点数 */
    for (lie = start_lie; lie <= end_lie; lie++) {
        White_Lie[lie][1] = 0;
        White_Lie[lie][0] = 0;
        White_Lie[lie][2] = 0;
        for (hang = MT9V03X_H - 1; hang >= 0; hang--) {
            if (mt9v03x_image_TwoValues[hang][lie] == 0) // 在某列找到黑点
            {
                if (White_Lie[lie][1] != 0) // 起始点已记录
                {
                    White_Lie[lie][2] = hang + 1;
                }
            } else // 找到的是白点
            {
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

    /* 从左到右找右边最长白列 */
    /* Longest_WhiteLie_R[0] = 0;
    Longest_WhiteLie_R[1] = 0;
    for (lie = start_lie; lie <= end_lie; lie++) {
        if (White_Lie[lie][0] >= Longest_WhiteLie_R[0] && White_Lie[lie][1] >= MT9V03X_H - 7) {
            Longest_WhiteLie_R[0] = White_Lie[lie][0]; // 记录长度
            Longest_WhiteLie_R[1] = lie;               // 记录下标
        }
    } */

    /* 从右最长白列到右找角点 */
    Island_State3_Point[0] = 0;
    for (lie = start_lie; lie <= MT9V03X_W - 5; lie++) {
        if (White_Lie[lie][2] - White_Lie[lie - 3][2] >= 6 &&
            White_Lie[lie][2] - White_Lie[lie - 4][2] >= 8 &&
            White_Lie[lie][2] - White_Lie[lie + 2][2] >= 4 &&
            White_Lie[lie][2] - White_Lie[lie + 3][2] >= 6) {
            Island_State3_Point[0] = White_Lie[lie][2];
            Island_State3_Point[1] = lie;
        }
    }

    /* 从左到右找右边最长白列 */
    Longest_WhiteLie_R[0] = 0;
    Longest_WhiteLie_R[1] = 0;
    for (lie = Island_State3_Point[1]; lie <= MT9V03X_W - 5; lie++) {
        if (White_Lie[lie][0] >= Longest_WhiteLie_R[0]) {
            Longest_WhiteLie_R[0] = White_Lie[lie][0]; // 记录长度
            Longest_WhiteLie_R[1] = lie;               // 记录下标
        }
    }

    /* 搜索截止行设为最长白列长度 */
    Search_Stop_Line = Longest_WhiteLie_R[0];

    for (hang = MT9V03X_H - 1; hang >= MT9V03X_H - Search_Stop_Line; hang--) {
        // for (hang = MT9V03X_H - 1; hang >= 1; hang--) {
        /* 从左最长白列向左寻线 */
        for (lie = Longest_WhiteLie_R[1]; lie >= 0 + 2; lie--) {
            if (mt9v03x_image_TwoValues[hang][lie] == 255 && mt9v03x_image_TwoValues[hang][lie - 1] == 0 && mt9v03x_image_TwoValues[hang][lie - 2] == 0) {
                L_Line[hang] = lie;
                L_Flag[hang] = 1;
                break;
            } else if (lie <= 2) // 没找到，则把图像边界当做边线
            {
                L_Line[hang] = lie;
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
                R_Line[hang] = lie;
                R_Flag[hang] = 0;
                break;
            }
        }
    }
}

void Image_Island_Dect()
{
    Continuity_Change(MT9V03X_H - 7, 10); // 判断赛道连续性
    if (Island_State == 0) {
        if (Island_Flag_R == 0) {
            if (Continuity_Change_Flag_R != 0 && Continuity_Change_Flag_L == 0 && Both_LostLine_Time < 20) // 如果满足右边线撕裂，左边线连续，丢线数小于阈值
            {
                Find_Right_Down_Point(MT9V03X_H - 1, 20); // 找右下角点
                /* 判定右环 */
                if (Right_Down_Line >= 30) // 限定角点出现的位置
                {
                    Island_Flag_R = 1; // 判断进入环岛
                    Island_State  = 1;
                } else {
                    Island_Flag_R = 0;
                    Island_State  = 0;
                }
            }
        }
    } else if (Island_State == 1) // 判断进入环岛后,右下方边界未消失
    {
        if (Island_Flag_R == 1) // 右环
        {
            Monotonicity_Change_Right(30, 5);
            if (Monotonicity_Change_Line_R != 0) {
                /* 下面是连点补线 */
                // Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R,R_Line[R_Down_Point],R_Down_Point);
                /* 下面是左边界对称补线 */
                Island_Add_Line_R(Monotonicity_Change_Line_R, MT9V03X_H, L_Line_K(Monotonicity_Change_Line_R)); // 直接拉到最底部
            }
            /* 下方边界消失,进入状态2 */
            if (Boundry_Start_R < 60) {
                Island_State = 2;
            }
        }
    } else if (Island_State == 2) // 判断进入环岛后,右下方边界消失
    {
        Monotonicity_Change_Right(30, 5);
        if (Monotonicity_Change_Line_R != 0) {
            /* 下面是连点补线 */
            // Right_Add_Line(R_Line[Monotonicity_Change_Line_R], Monotonicity_Change_Line_R,R_Line[R_Down_Point],R_Down_Point);
            /* 下面是左边界对称补线 */
            Island_Add_Line_R(Monotonicity_Change_Line_R, MT9V03X_H, L_Line_K(Monotonicity_Change_Line_R)); // 直接拉线到最底部
        }
        if (Boundry_Start_R >= MT9V03X_H - 5 || Monotonicity_Change_Line_R > 50) // 当圆弧靠下或者下方不丢线
        {
            Island_State = 3;
        }
    } else if (Island_State == 3) {
        /* 手动调整标志位 */
        key_scanner();
        if (key_get_state(KEY_1) == KEY_SHORT_PRESS) {
            Island_State  = 0;
            Island_Flag_R = 0;
        }
        Island_State_3_Search_Line();
        Left_Add_Line(Island_State3_Point[1], Island_State3_Point[0], 0, MT9V03X_H);
        if (Island_State3_Point[0]>=MT9V03X_H-8)
        {
            Island_State = 4;
        }
    }
    else if (Island_State==4)
    {
        /* if(陀螺仪)
        {
            Island_State = 5;
        } */
    }
    else if (Island_State==5)
    {
        
    }
    
    ips200_show_int(0, 210, Island_State, 2);
    ips200_show_int(40, 210, Island_Flag_R, 2);
}
