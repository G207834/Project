#include "Image.h"
#include "math.h"
uint8 mt9v03x_image_TwoValues[MT9V03X_H][MT9V03X_W] = {0}; // 二值化后的数组
volatile int Longest_WhiteLie_L[2]                  = {0}; // 左最长白列，[0]白列长度，[1]白列位置，第几列
volatile int Longest_WhiteLie_R[2]                  = {0}; // 右最长白列，[0]白列长度，[1]白列位置，第几列
volatile int Search_Stop_Line                       = 0;   // 搜索截止行的长度，要得到坐标用高度减去此值
volatile int White_Lie[MT9V03X_W]                   = {0}; // 每列的白列长度
volatile int L_Line[MT9V03X_H]                      = {0}; // 左边线数组
volatile int R_Line[MT9V03X_H]                      = {0}; // 右边线数组
volatile int L_Flag[MT9V03X_H]                      = {0}; // 寻到左边线状态
volatile int R_Flag[MT9V03X_H]                      = {0}; // 寻到左边线状态
volatile int R_LostLine_Time                        = 0;   // 右边线丢线数
volatile int L_LostLine_Time                        = 0;   // 左边线丢线数
volatile int Both_LostLine_Time                     = 0;   // 双边边线丢线数
volatile int Boundry_Start_L                        = 0;   // 左边第一个边界点，第几行
volatile int Boundry_Start_R                        = 0;   // 右边第一个边界点，第几行

volatile int Road_Wide[MT9V03X_H] = {0}; // 赛道宽度

/* 角点 */
int L_Down_Point = 0;
int R_Down_Point = 0;
int L_Up_Point   = 0;
int R_Up_Point   = 0;

extern volatile float Err; // 摄像头误差

/* 标志位元素 */
int Straight_Flag = 0; // 直道标志位
int Island_State  = 0; // 环岛标志位
int Ramp_Flag     = 0; // 坡道标志位
int Cross_Flag    = 0; // 十字标志位

/* 图像二值化，固定阈值，原图0-255>黑-白,二值图0or255>黑—白*/
void Image_Change_TwoValues(uint8 value)
{
    for (int hang = 0; hang < MT9V03X_H; hang++) {
        for (int lie = 0; lie < MT9V03X_W; lie++) {
            if (mt9v03x_image[hang][lie] < value) {
                mt9v03x_image_TwoValues[hang][lie] = 0;
            } else {
                mt9v03x_image_TwoValues[hang][lie] = 255;
            }
        }
    }
}

/* 动态阈值，大津法 */

/* 图像中线 93 H*/
/* 求赛道左右边线 >左右边线*/
/* 黑白交界线 */
/* 化简后，就找一行，赛道中点 */
/* 从中间往两边找 */

void Image_LongestWhite_SearchLine()
{
    int start_lie = 20; // 最长白列搜索区间，图像左右减去此长度
    int end_lie   = MT9V03X_W - 1 - start_lie;
    int hang      = 0; // 行
    int lie       = 0; // 列
    /* 从左到右，从下到上遍历数组，记录每列白点数 */
    for (lie = start_lie; lie <= end_lie; lie++) {
        White_Lie[lie] = 0; // 清零
        for (hang = MT9V03X_H - 1; hang >= 0; hang--) {
            if (mt9v03x_image_TwoValues[hang][lie] == 0) {
                break;
            } else {
                White_Lie[lie]++;
            }
        }
    }
    /* 从左到右找左最长白列 */
    Longest_WhiteLie_L[0] = 0;
    Longest_WhiteLie_L[1] = 0;
    for (lie = start_lie; lie <= end_lie; lie++) {
        if (White_Lie[lie] > Longest_WhiteLie_L[0]) {
            Longest_WhiteLie_L[0] = White_Lie[lie]; // 记录长度
            Longest_WhiteLie_L[1] = lie;            // 记录下标
        }
    }
    /* 从右到左找右最长白列 */
    Longest_WhiteLie_R[0] = 0;
    Longest_WhiteLie_R[1] = 0;
    for (lie = end_lie; lie >= Longest_WhiteLie_L[1]; lie--) {
        if (White_Lie[lie] > Longest_WhiteLie_R[0]) {
            Longest_WhiteLie_R[0] = White_Lie[lie]; // 记录长度
            Longest_WhiteLie_R[1] = lie;            // 记录下标
        }
    }

    /* 搜索截止行设为最长白列长度 */
    Search_Stop_Line = Longest_WhiteLie_L[0];

    // for (hang = MT9V03X_H - 1; hang >= MT9V03X_H - Search_Stop_Line; hang--) {
    for (hang = MT9V03X_H - 1; hang >= 1; hang--) {
        /* 从左最长白列向左寻线 */
        for (lie = Longest_WhiteLie_L[1]; lie >= 0 + 2; lie--) {
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

    /* 将数据清零 */
    R_LostLine_Time    = 0;
    L_LostLine_Time    = 0;
    Both_LostLine_Time = 0;
    Boundry_Start_L    = 0;
    Boundry_Start_R    = 0;
    /* 统计处理 */
    for (hang = MT9V03X_H - 1; hang >= 0; hang--) {
        /* 统计丢线数 */
        if (L_Flag[hang] == 0) {
            L_LostLine_Time++;
        }
        if (R_Flag[hang] == 0) {
            R_LostLine_Time++;
        }
        if (L_Flag[hang] == 0 && R_Flag[hang] == 0) {
            Both_LostLine_Time++;
        }
        /* 统计起始边界点所在行 */
        if (Boundry_Start_L == 0 && L_Flag[hang] == 1) {
            Boundry_Start_L = hang;
        }
        if (Boundry_Start_R == 0 && R_Flag[hang] == 1) {
            Boundry_Start_R = hang;
        }
        /* 计算赛道宽度 */
        Road_Wide[hang] = R_Line[hang] - L_Line[hang];
    }
}

/* 直道检测 */

/* 中线分布方差
视野远处到近处的斜率 */
void Straight_Detect()
{
    Straight_Flag = 0;
    if (Search_Stop_Line >= 65) // 截止行很远
    {
        if (Boundry_Start_L >= 65 && Boundry_Start_R >= 65) // 起始点靠下
        {
            if (-5 <= Err && Err <= 5) {
                Straight_Flag = 1;
            }
        }
    }
}

/* 寻找下角点，十字判断 */
void Find_Down_Point(int start, int end)
{
    int temp, hang;
    /* 清除标志位 */
    R_Down_Point = 0;
    L_Down_Point = 0;
    if (start < end) {
        temp  = start;
        start = end;
        end   = temp;
    }
    if (start >= MT9V03X_H - 1 - 5) // 下面五行不能用
    {
        start = MT9V03X_H - 1 - 6;
    }
    if (end <= MT9V03X_H - 1 - Search_Stop_Line) {
        end = MT9V03X_H - 1 - Search_Stop_Line;
    }
    if (end <= 5) {
        end = 5;
    }
    for (hang = start; hang >= end; hang--) {
        if (L_Down_Point == 0 &&
            abs(L_Line[hang] - L_Line[hang + 1]) <= 5 &&
            abs(L_Line[hang + 1] - L_Line[hang + 2]) <= 5 &&
            abs(L_Line[hang + 2] - L_Line[hang + 3]) <= 5 && // 下部撕裂不大
            (L_Line[hang] - L_Line[hang - 2]) >= 5 &&        // 上部撕裂
            (L_Line[hang] - L_Line[hang - 3]) >= 10 &&
            (L_Line[hang] - L_Line[hang - 4]) >= 10) {
            L_Down_Point = hang;
        }
        if (R_Down_Point == 0 &&
            abs(R_Line[hang] - R_Line[hang + 1]) <= 5 &&
            abs(R_Line[hang + 1] - R_Line[hang + 2]) <= 5 &&
            abs(R_Line[hang + 2] - R_Line[hang + 3]) <= 5 && // 下部撕裂不大
            (R_Line[hang] - R_Line[hang - 2]) <= -5 &&       // 上部撕裂
            (R_Line[hang] - R_Line[hang - 3]) <= -10 &&
            (R_Line[hang] - R_Line[hang - 4]) <= -10) {
            R_Down_Point = hang;
        }
        if (L_Down_Point != 0 && R_Down_Point != 0) // 两个都找到，退出
        {
            break;
        }
    }
}

/* 寻找上角点，十字判断 */
void Find_Up_Point(int start, int end)
{
    int temp, hang;
    /* 清除标志位 */
    R_Up_Point = 0;
    L_Up_Point = 0;
    if (start < end) {
        temp  = start;
        start = end;
        end   = temp;
    }
    if (start >= MT9V03X_H - 1 - 5) // 下面五行不能用
    {
        start = MT9V03X_H - 1 - 6;
    }
    if (end <= MT9V03X_H - 1 - Search_Stop_Line) {
        end = MT9V03X_H - 1 - Search_Stop_Line;
    }
    if (end <= 5) {
        end = 5;
    }
    for (hang = start; hang >= end; hang--) {
        // for (hang = end; hang <= start; hang++) {
        if (L_Up_Point == 0 &&
            abs(L_Line[hang] - L_Line[hang - 1]) <= 5 &&
            abs(L_Line[hang - 1] - L_Line[hang - 2]) <= 5 &&
            abs(L_Line[hang - 2] - L_Line[hang - 3]) <= 5 && // 上部撕裂不大
            (L_Line[hang] - L_Line[hang + 2]) >= 5 &&        // 下部撕裂,撕裂阈值可改
            (L_Line[hang] - L_Line[hang + 3]) >= 8 &&
            (L_Line[hang] - L_Line[hang + 4]) >= 8) {
            L_Up_Point = hang;
        }
        if (R_Up_Point == 0 &&
            abs(R_Line[hang] - R_Line[hang - 1]) <= 5 &&
            abs(R_Line[hang - 1] - R_Line[hang - 2]) <= 5 &&
            abs(R_Line[hang - 2] - R_Line[hang - 3]) <= 5 && // 上部撕裂不大
            (R_Line[hang] - R_Line[hang + 2]) <= -5 &&       // 下部撕裂，撕裂阈值需要实测
            (R_Line[hang] - R_Line[hang + 3]) <= -8 &&
            (R_Line[hang] - R_Line[hang + 4]) <= -8) {
            R_Up_Point = hang;
        }
        if (L_Up_Point != 0 && R_Up_Point != 0) // 两个都找到，退出
        {
            break;
        }
    }
    if (abs(R_Up_Point - L_Up_Point) >= 40) // 纵向撕裂过大，视为误判
    {
        R_Up_Point = 0;
        L_Up_Point = 0;
    }
}

/* 边界显示，直接修改二值化图像，只允许调参调用 */
void Image_Show_Boundry(void)
{
    int hang = 0;
    for (hang = MT9V03X_H - 1; hang >= MT9V03X_H - Search_Stop_Line; hang--) // 从最底下往上扫描
    {
        mt9v03x_image_TwoValues[hang][L_Line[hang] + 1]                   = 0;
        mt9v03x_image_TwoValues[hang][(L_Line[hang] + R_Line[hang]) >> 1] = 0;
        mt9v03x_image_TwoValues[hang][R_Line[hang] - 1]                   = 0;
    }

    // 在屏幕理论中线处显示红线，用于调整摄像头
    // ips200_draw_line ( MT9V03X_W/2, MT9V03X_H-10, MT9V03X_W/2, MT9V03X_H, RGB565_RED);
}

void Left_Add_Line(int x1, int y1, int x2, int y2) // 左补线,补的是边界
{
    int i, max, a1, a2;
    int hx;
    if (x1 >= MT9V03X_W - 1) // 起始点位置校正，排除数组越界的可能
        x1 = MT9V03X_W - 1;
    else if (x1 <= 0)
        x1 = 0;
    if (y1 >= MT9V03X_H - 1)
        y1 = MT9V03X_H - 1;
    else if (y1 <= 0)
        y1 = 0;
    if (x2 >= MT9V03X_W - 1)
        x2 = MT9V03X_W - 1;
    else if (x2 <= 0)
        x2 = 0;
    if (y2 >= MT9V03X_H - 1)
        y2 = MT9V03X_H - 1;
    else if (y2 <= 0)
        y2 = 0;
    a1 = y1;
    a2 = y2;
    if (a1 > a2) // 坐标互换
    {
        max = a1;
        a1  = a2;
        a2  = max;
    }
    for (i = a1; i <= a2; i++) // 根据斜率补线即可
    {
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= MT9V03X_W)
            hx = MT9V03X_W;
        else if (hx <= 0)
            hx = 0;
        L_Line[i] = hx;
    }
}

void Right_Add_Line(int x1, int y1, int x2, int y2) // 右补线,补的是边界
{
    int i, max, a1, a2;
    int hx;
    if (x1 >= MT9V03X_W - 1) // 起始点位置校正，排除数组越界的可能
        x1 = MT9V03X_W - 1;
    else if (x1 <= 0)
        x1 = 0;
    if (y1 >= MT9V03X_H - 1)
        y1 = MT9V03X_H - 1;
    else if (y1 <= 0)
        y1 = 0;
    if (x2 >= MT9V03X_W - 1)
        x2 = MT9V03X_W - 1;
    else if (x2 <= 0)
        x2 = 0;
    if (y2 >= MT9V03X_H - 1)
        y2 = MT9V03X_H - 1;
    else if (y2 <= 0)
        y2 = 0;
    a1 = y1;
    a2 = y2;
    if (a1 > a2) // 坐标互换
    {
        max = a1;
        a1  = a2;
        a2  = max;
    }
    for (i = a1; i <= a2; i++) // 根据斜率补线即可
    {
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= MT9V03X_W)
            hx = MT9V03X_W;
        else if (hx <= 0)
            hx = 0;
        R_Line[i] = hx;
    }
}

void Lengthen_Left_Boundry(int start, int end)
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
    }

    else {
        k = (float)(L_Line[start] - L_Line[start - 4]) / 5.0; // 这里的k是1/斜率
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

void Lengthen_Right_Boundry(int start, int end)
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
        k = (float)(R_Line[start] - R_Line[start - 4]) / 5.0; // 这里的k是1/斜率
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

void Image_Cross_Detect()
{
    int down_search_start = 0; // 下点搜索开始行
    Cross_Flag            = 0;
    if (Island_State == 0 && Ramp_Flag == 0) // 与环岛互斥开
    {
        L_Up_Point = 0;
        R_Up_Point = 0;
        if (Both_LostLine_Time >= 0) // 十字必定有双边丢线，在有双边丢线的情况下再开始找角点
        {
            Find_Up_Point(MT9V03X_H - 1, 0);
            if (L_Up_Point == 0 && L_Up_Point == 0) // 只要没有同时找到两个上点，直接结束
            {
                return;
            }
        }
        if (L_Up_Point != 0 && R_Up_Point != 0) // 找到两个上点，就找到十字了
        {
            Cross_Flag        = 1;                                                 // 对应标志位，便于各元素互斥掉
            down_search_start = L_Up_Point > R_Up_Point ? L_Up_Point : R_Up_Point; // 用两个上拐点坐标靠下者作为下点的搜索上限
            Find_Down_Point(MT9V03X_H - 5, down_search_start + 2);                 // 在上拐点下2行作为下点的截止行
            if (L_Down_Point <= L_Up_Point) {
                L_Down_Point = 0; // 下点不可能比上点还靠上
            }
            if (R_Down_Point <= R_Up_Point) {
                R_Down_Point = 0; // 下点不可能比上点还靠上
            }
            if (L_Down_Point != 0 && R_Down_Point != 0) { // 四个点都在，无脑连线，这种情况显然很少
                Left_Add_Line(L_Line[L_Up_Point], L_Up_Point, L_Line[L_Down_Point], L_Down_Point);
                Right_Add_Line(R_Line[R_Up_Point], R_Up_Point, R_Line[R_Down_Point], R_Down_Point);
            } else if (L_Down_Point == 0 && R_Down_Point != 0) // 11//这里使用的都是斜率补线
            {                                                  // 三个点                                     //01
                Lengthen_Left_Boundry(L_Up_Point - 1, MT9V03X_H - 1);
                Right_Add_Line(R_Line[R_Up_Point], R_Up_Point, R_Line[R_Down_Point], R_Down_Point);
            } else if (L_Down_Point != 0 && R_Down_Point == 0) // 11
            {                                                  // 三个点                                     //10
                Left_Add_Line(L_Line[L_Up_Point], L_Up_Point, L_Line[L_Down_Point], L_Down_Point);
                Lengthen_Right_Boundry(R_Up_Point - 1, MT9V03X_H - 1);
            } else if (L_Down_Point == 0 && R_Down_Point == 0) // 11
            {                                                  // 就俩上点                                   //00
                Lengthen_Left_Boundry(L_Up_Point - 1, MT9V03X_H - 1);
                Lengthen_Right_Boundry(R_Up_Point - 1, MT9V03X_H - 1);
            }
        } else {
            Cross_Flag = 0;
        }
    }
    // 角点相关变量，debug使用
    // ips200_showint(0,12,Cross_Flag);
    //    ips200_showint(0,13,Island_State);
    //    ips200_showint(50,12,L_Up_Point);
    //    ips200_showint(100,12,R_Up_Point);
    //    ips200_showint(50,13,L_Down_Point);
    //    ips200_showint(100,13,R_Down_Point);
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
/* int Find_Right_Down_Point(int start, int end) // 找四个角点，返回值是角点所在的行数
{
    int i, t;
    int right_down_line = 0;
    if (R_LostLine_Time >= 0.9 * MT9V03X_H) // 大部分都丢线，没有拐点判断的意义
        return right_down_line;
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
        if (right_down_line == 0 &&                // 只找第一个符合条件的点
            abs(R_Line[i] - R_Line[i + 1]) <= 5 && // 角点的阈值可以更改
            abs(R_Line[i + 1] - R_Line[i + 2]) <= 5 &&
            abs(R_Line[i + 2] - R_Line[i + 3]) <= 5 &&
            (R_Line[i] - R_Line[i - 2]) <= -5 &&
            (R_Line[i] - R_Line[i - 3]) <= -10 &&
            (R_Line[i] - R_Line[i - 4]) <= -10) {
            right_down_line = i; // 获取行数即可
            break;
        }
    }
    return right_down_line;
} */
