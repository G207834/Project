#include "zf_common_headfile.h"
#include "pid.h"
#include "servo.h"
#include "Control.h"
#include "math.h"
// PID_t PID_Servo = {
//     .Kp = 2.56,
//     .Ki = 0,   // I
//     .Kd = 0.1, // Ds

//     .OutMax = 100,
//     .OutMin = -100,

//     .ErrorIntMax = 0,
//     .ErrorIntMin = 0,
// };

    // .Kp = 1.9,
    // .Ki = 0,   // I
    // .Kd = 0.8, // Ds
    // .Gkd = 0.0,
    // .Kp2 = 0,
PID_t PID_Servo = {
    .Kp = 1.9,
    .Ki = 0,   // I
    .Kd = 0.05, // Ds0.04
    .Gkd = 0.1,
    .Kp2 = 0.0015,

    .OutMax = 110,
    .OutMin = -130,

    .ErrorIntMax = 0,
    .ErrorIntMin = 0,
};

void Servo_Init(void)
{
    pwm_init(Servo_PWM, Servo_FREQ, Servo_MID);
}

// 舵机打角度给整形吧，float类型的话进来函数里面pwm_set_duty的函数参数也会强制转换为uint32，而且角度浮点型的话零点几的话角度那么小，也多不了多少
void Servo_Set(uint32 servo_motor_duty)
{
    pwm_set_duty(Servo_PWM, servo_motor_duty);
}

// 动态PD
// void Dynamic_PD(void)
// {
//     PID_Servo.Kp = 2.1 + fabsf(Err) * 0.028;
// }

// 简介：舵机闭环转向
void Servo_Control(float error)
{
    Get_Gyro_ICM42688();
    Servo_Set(Servo_PD(&PID_Servo, error) + Servo_MID);
}
