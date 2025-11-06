#include "zf_common_headfile.h"
#include "math.h"
#include "pid.h"
#include "icm24688.h"

#define EPSILON 0.003


// // 不完全微分：避免因为图像噪声导致舵机震荡
// //@简介：位置式PD控制，用于舵机
// // 参数：pid结构体参数
// // 参数：从摄像头获取的误差
// // 返回值：对舵机的控制角度数值
// float Servo_PD(PID_t *Pid, float get_error)
// {
//     // 获取误差
//     Pid->Error      = get_error;
//     Pid->Last_Error = Pid->Error;

//     // pd调控
//     Pid->Out_p = Pid->Kp * Pid->Error;
//     Pid->Out_d = (1 - Pass) * Pid->Kd * (Pid->Error - Pid->Last_Error) + Pass * Pid->Out_d;
//     Pid->Out   = Pid->Out_p + Pid->Out_d;

//     // 输出限幅
//     if (Pid->Out > Pid->OutMax) {
//         Pid->Out = Pid->OutMax;
//     }
//     if (Pid->Out < Pid->OutMin) {
//         Pid->Out = Pid->OutMin;
//     }
//     //ips200_show_float(150, 210, Pid->Out, 2, 4);
//     return Pid->Out;
// }


//@简介：位置式PD控制，用于舵机，陀螺仪辅助转向
//参数：pid结构体参数
//参数：从摄像头获取的误差
//返回值：对舵机的控制角度数值
float Kp2out;
float Gkdout;
float out;
float Pout;
float Servo_PD(PID_t *Pid,float get_error)
{

    //获取误差
    Pid->Error = get_error;
    Pid->Last_Error = Pid->Error;

    Kp2out = Pid->Error * fabsf(Pid->Error) * Pid->Kp2;
    Gkdout = Pid->Gkd * icm42688_gyro_x;
    //Kd项的效果不是为了完全抑制抖动的，它本身还有预测功能
    //Gd项反馈车身状态，用来抑制抖动更适合，效果也单一
    Pid->Out_p = Pid->Kp * Pid->Error + Pid->Error * fabsf(Pid->Error) * Pid->Kp2;
    // Pout = Pid->Out_p;
    Pid->Out_d = Pid->Kd * (Pid->Error - Pid->Last_Error) - Pid->Gkd * icm42688_gyro_x;
    Pid->Out = Pid->Out_p +  Pid->Out_d;
    // out =  Pid->Out;

    // 输出限幅  
    if (Pid->Out > Pid->OutMax)
    {
        Pid->Out = Pid->OutMax;
    }
    if (Pid->Out < Pid->OutMin)
    {
        Pid->Out = Pid->OutMin;
    }

    return Pid->Out;
}

// 微分先行：针对目标值的大幅度变化时，会导致误差大幅度变化，微分项会突然输出一个很大的调控力，目标数值频繁切换不利于系统稳定
// 简介：增量式PID控制，用于电机
// 参数：pid结构体参数
// 参数：从编码器获取的实际数值
// 参数：目标速度
// 返回值：对电机的pwm控制数值

float Motor_PID(PID_t *Pid, float get_Actual, float target_Speed)
{
        
    Pid->Target = target_Speed;
    Pid->Actual = get_Actual;

    // 获取误差
    Pid->LL_Error   = Pid->Last_Error;
    Pid->Last_Error = Pid->Error;
    Pid->Error      = Pid->Target - Pid->Actual;

    // 误差积分
    // if (fabs(Pid->Ki) > EPSILON) {
    //     Pid->ErrorInt += Pid->Error;
    // } else {
    //     Pid->ErrorInt = 0;
    // }
    // /* 积分限幅 */
    // if (Pid->ErrorInt > Pid->ErrorIntMax) {
    //     Pid->ErrorInt = Pid->ErrorIntMax;
    // }
    // if (Pid->ErrorInt < Pid->ErrorIntMin) {
    //     Pid->ErrorInt = Pid->ErrorIntMin;
    // }

    // PID调控
    Pid->Out_p = Pid->Kp * (Pid->Error - Pid->Last_Error);
    Pid->Out_i = Pid->Ki * (Pid->Error);
    Pid->Out_d = Pid->Kd * (Pid->Error - 2 * Pid->Last_Error + Pid->LL_Error);

    Pid->Out += (Pid->Out_p + Pid->Out_i + Pid->Out_d);

    // 输出限幅
    if (Pid->Out > Pid->OutMax) {
        Pid->Out = Pid->OutMax;
    }
    if (Pid->Out < Pid->OutMin) {
        Pid->Out = Pid->OutMin;
    }


    return Pid->Out;
}

// 舵机打角度给整形吧，float类型的话进来函数里面pwm_set_duty的函数参数也会强制转换为uint32，而且角度浮点型的话零点几的话角度那么小，也多不了多少
// 电机的也一样，进入pwm_set_duty的函数参数也会强制转换为uint32
