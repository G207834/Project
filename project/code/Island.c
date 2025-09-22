#include "zf_common_headfile.h"
#include "Island.h"

extern uint8 mt9v03x_image_TwoValues[MT9V03X_H][MT9V03X_W] = {0}; // 二值化后的数组
extern volatile int Longest_WhiteLie_L[2]                  = {0}; // 左最长白列，[0]白列长度，[1]白列位置，第几列
extern volatile int Longest_WhiteLie_R[2]                  = {0}; // 右最长白列，[0]白列长度，[1]白列位置，第几列
extern volatile int Search_Stop_Line                       = 0;   // 搜索截止行的长度，要得到坐标用高度减去此值
extern volatile int White_Lie[MT9V03X_W][3]                = {0}; // 0白列长度，1白列起始行数，2白列终止行数
extern volatile int L_Line[MT9V03X_H]                      = {0}; // 左边线数组
extern volatile int R_Line[MT9V03X_H]                      = {0}; // 右边线数组
extern volatile int L_Flag[MT9V03X_H]                      = {0}; // 寻到左边线状态
extern volatile int R_Flag[MT9V03X_H]                      = {0}; // 寻到左边线状态
extern volatile int R_LostLine_Time                        = 0;   // 右边线丢线数
extern volatile int L_LostLine_Time                        = 0;   // 左边线丢线数
extern volatile int Both_LostLine_Time                     = 0;   // 双边边线丢线数
extern volatile int Boundry_Start_L                        = 0;   // 左边第一个边界点，第几行
extern volatile int Boundry_Start_R                        = 0;   // 右边第一个边界点，第几行

extern int Road_Wide[MT9V03X_H] = {0}; // 赛道宽度

/* 角点 */
extern int L_Down_Point = 0;
extern int R_Down_Point = 0;
extern int L_Up_Point   = 0;
extern int R_Up_Point   = 0;

extern volatile float Err; // 摄像头误差

/* 标志位元素 */
extern int Straight_Flag = 0; // 直道标志位
extern int Island_State  = 0; // 环岛标志位
extern int Island_Flag_L = 0; // 环岛标志位
extern int Island_Flag_R = 0; // 环岛标志位
extern int Ramp_Flag     = 0; // 坡道标志位
extern int Cross_Flag    = 0; // 十字标志位

/* 环岛相关 */
uint8 Continuity_Change_Flag_L = 0;
uint8 Continuity_Change_Flag_R = 0;

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

void Image_Island_Dect()
{
    Continuity_Change(MT9V03X_H - 7, 10);
    if (Island_Flag_L == 0) {
        if (Continuity_Change_Flag_R != 0 &&Continuity_Change_Flag_L == 0 &&Both_LostLine_Time < 20;) // 如果满足右边线撕裂，左边线连续，丢线数小于阈值
        {
        }
    }
}
