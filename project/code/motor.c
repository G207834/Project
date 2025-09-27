#include "zf_common_headfile.h"
#include "pid.h"
#include "motor.h"


// 电机参数
motor motor_R;
motor motor_L;

/******电机的PID参数******/
PID_t PID_Motor_R = {
    .Target = 0,
    .Actual = 0,

    .Kp = 0,
    .Ki = 0, // I
    .Kd = 0, // D

    .OutMax = 0,
    .OutMin = 0,

    .ErrorIntMax = 0,
    .ErrorIntMin = 0,
};

PID_t PID_Motor_L = {
    .Target = 0,
    .Actual = 0,

    .Kp = 0,
    .Ki = 0, // I
    .Kd = 0, // D

    .OutMax = 0,
    .OutMin = 0,

    .ErrorIntMax = 0,
    .ErrorIntMin = 0,
};

// 简介：左右电机初始化
//  //参数：无
//  //返回：无
void Motor_Init(void)
{
    pwm_init(PWM_L1, 17 * 1000, 0);
    pwm_init(PWM_L2, 17 * 1000, 0); // PWM 通道初始化频率 17KHz 占空比初始为 0

    pwm_init(PWM_R1, 17 * 1000, 0);
    pwm_init(PWM_R2, 17 * 1000, 0); // PWM 通道初始化频率 17KHz 占空比初始为 0
}

// 简介：设置左右电机的占空比，实现正反转
// 参数：左右电机的占空比
// 返回：无
void Set_Motor_PWM(int32 motorL, int32 motorR)
{
    if (motorL >= 0) // 左侧正转
    {
        pwm_set_duty(PWM_L1, motorL * (PWM_DUTY_MAX / 100)); // 计算占空比
        pwm_set_duty(PWM_L2, 0);                             // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
    } else                                                   // 左侧反转
    {
        pwm_set_duty(PWM_L1, 0);                                // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
        pwm_set_duty(PWM_L2, (-motorL) * (PWM_DUTY_MAX / 100)); // 计算占空比
    }
    if (motorR >= 0) // 左侧正转
    {
        pwm_set_duty(PWM_R1, motorR * (PWM_DUTY_MAX / 100)); // 计算占空比
        pwm_set_duty(PWM_R2, 0);                             // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
    } else                                                   // 左侧反转
    {
        pwm_set_duty(PWM_R1, 0);                                // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
        pwm_set_duty(PWM_R2, (-motorR) * (PWM_DUTY_MAX / 100)); // 计算占空比
    }
}

// 简介：电机闭环控制
// 参数：左右电机的目标速度
// 返回：无
// 备注：放定时中断里面执行
// 备注：输出速度会随着定时中断时间而变化，进行限幅
void Motor_Control(int16 Speed_l, int16 Speed_R)
{

    Encoder_GetData();
    Set_Motor_PWM(Motor_PID(&PID_Motor_L, motor_L.encoder_speed, Speed_l), Motor_PID(&PID_Motor_R, motor_R.encoder_speed, Speed_R));
}

/********************************************编码器******************************************************************************************/

// 简介：编码器初始化
// 参数：无
// 返回：无
void Encoder_Init(void)
{
    // 左编码器
    encoder_quad_init(ENCODER_L, ENCODER_L_A, ENCODER_L_B); // 初始化编码器模块与引脚 正交解码编码器模式
    // 右编码器
    encoder_quad_init(ENCODER_R, ENCODER_R_A, ENCODER_R_B); // 初始化编码器模块与引脚 正交解码编码器模式
}
// 简介：编码器数据获取
// 参数：无
// 返回：无
void Encoder_GetData(void)
{

    // 左编码器数据获取
    motor_L.encoder_raw   = -encoder_get_count(ENCODER_L);
    motor_L.encoder_speed = motor_L.encoder_speed * X + motor_L.encoder_raw * (1 - X);
    encoder_clear_count(ENCODER_L);

    // 右编码器数据获取
    motor_R.encoder_raw   = encoder_get_count(ENCODER_R);
    motor_R.encoder_speed = motor_R.encoder_speed * X + motor_R.encoder_raw * (1 - X);
    encoder_clear_count(ENCODER_R);
}
