#include "zf_common_headfile.h"
#include "pid.h"
#include "servo.h"
PID_t PID_Servo = {
    .Kp = 1.6,
    .Ki = 0,   // I
    .Kd = 0.8, // Ds

    .OutMax = 70,
    .OutMin = -95,

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

    /*  if (servo_motor_duty < Servo_L_MAX) { servo_motor_duty = Servo_L_MAX; }
     if (servo_motor_duty > Servo_R_MAX) { servo_motor_duty = Servo_R_MAX; }
  */
    pwm_set_duty(Servo_PWM, servo_motor_duty);
}

// 简介：舵机闭环转向
void Servo_Control(float error)
{
    Servo_Set(Servo_PD(&PID_Servo, error) + Servo_MID);
}
