#include "Control.h"
#include "motor.h"
#include "servo.h"
volatile float Err;                    // 摄像头误差
extern volatile int Search_Stop_Line;  // 搜索截止行
extern volatile int L_Line[MT9V03X_H]; // 左边线数组
extern volatile int R_Line[MT9V03X_H]; // 右边线数组

// 加权控制
const int Weight[MT9V03X_H] =
    {
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1, // 图像最远端00 ——09 行权重
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1, // 图像最远端10 ——19 行权重
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        3,
        4,
        5, // 图像最远端20 ——29 行权重
        6,
        7,
        9,
        11,
        13,
        15,
        17,
        19,
        20,
        20, // 图像最远端30 ——39 行权重
        19,
        17,
        15,
        13,
        11,
        9,
        7,
        5,
        3,
        8, // 图像最远端40 ——49 行权重
        8,
        8,
        8,
        8,
        12,
        12,
        12,
        12,
        12,
        12,// 图像最远端50 ——59 行权重
        12,
        12,
        12,
        12,
        12,
        12,
        12,
        12,
        12,
        12, // 图像最远端60 ——69 行权重
}; // 权重只是选取某一范围内作为主要控制行，并且尽可能用上其他图像误差行
   // 参数没有固定范围，来源，方法，实际测试好用就可以
   // 因为实际使用的时候加权后又取平均，所以数据范围和平均，单行误差没什么区别
/* 求误差的加权平均数 */

void Err_Sum(void)
{
    int i;
    Err                = 0;
    float weight_count = 0;
    // 常规误差
    for (i = MT9V03X_H - 1; i >= MT9V03X_H - Search_Stop_Line - 1; i--) // 常规误差计算
    {
        Err += (MT9V03X_W / 2 - ((L_Line[i] + R_Line[i]) >> 1)) * Weight[i]; // 右移1位，等效除2
        weight_count += Weight[i];
    }
    Err = Err / weight_count;
    /* for (i = MT9V03X_H - 1; i >= MT9V03X_H - Search_Stop_Line - 1; i--) // 常规误差计算
    {
        Err += (MT9V03X_W / 2 - ((L_Line[i] + R_Line[i]) / 2.0)); // 右移1位，等效除2
    }
    Err = Err / 70; */
}
