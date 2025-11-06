#include "zf_common_headfile.h"
#include "pid.h"
#include "motor.h"
#include "Control.h"

// 电机参数
motor motor_R;
motor motor_L;

float L;
float R;

/******电机的PID参数******/
    // .Kp = 0.1,           2.5         6       8           2       2               6       6           6.3     7       7   7
    // .Ki = 0.15, // I     1.2         3       3.2         0.9     1.1             2.1     3.2         3.2     3.6     4   7
    // .Kd = 0.01, // D

PID_t PID_Motor_R = {
    .Target = 0,
    .Actual = 0,

    // .Kp = 9.6,//7.2
    // .Ki = 2.5, // I 4
    // .Kd = 0, // D

    // .Kp = 8.5,
    // .Ki = 7.8, // I
    // .Kd = 0, // D

    .Kp = 2.85,//1.7  1.85
    .Ki = 0.66, // I 0.65 0.66
    .Kd = 0, // D

    .OutMax = 5200,
    .OutMin = -5200,

    .ErrorIntMax = 200000,
    .ErrorIntMin = -200000,
};
// //
//     .Kp = 0.10,             4            4.5     4.9         9.2     9.2    9.2
//     .Ki = 0.18, //       1.5             2.1     2.1         2.1     2.5    5.5
    //     .Kd = 0.01, //
PID_t PID_Motor_L = {
    .Target = 0,
    .Actual = 0,

    // .Kp = 9.6,
    // .Ki = 2.5, // I
    // .Kd = 0, // D

    // .Kp = 14.2,
    // .Ki = 8.6, // I
    // .Kd = 0, // D

    .Kp = 2.65,//  1.65
    .Ki = 0.65, // I  0.68  0.65
    .Kd = 0, // D

    .OutMax =5000,
    .OutMin = -5000,

    .ErrorIntMax = 200000,
    .ErrorIntMin = -200000,
};

// 简介：左右电机初始化
// 参数：无
// 返回：无
void Motor_Init(void)
{
    gpio_init(DIR_L, GPO, GPIO_HIGH,
              GPO_PUSH_PULL);      // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17 * 1000, 0); // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_R, GPO, GPIO_HIGH,
              GPO_PUSH_PULL);      // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, 17 * 1000, 0); // PWM 通道初始化频率 17KHz 占空比初始为 0
}

// 简介：设置左右电机的占空比，实现正反转
// 参数：左右电机的占空比
// 返回：无
void Set_Motor_PWM(int32 motorL, int32 motorR)
{
    if (motorL >= 0) // 电机
    {
        gpio_high(DIR_L);
        pwm_set_duty(PWM_L, motorL);
    } else {
        gpio_low(DIR_L);
        pwm_set_duty(PWM_L, -motorL);
    }
    if (motorR >= 0) {
        gpio_high(DIR_R);
        pwm_set_duty(PWM_R, motorR);
    } else {
        gpio_low(DIR_R);
        pwm_set_duty(PWM_R, -motorR);
    }
}

// 简介：电机闭环控制
// 参数：左右电机的目标速度
// 返回：无
// 备注：放定时中断里面执行
// 备注：输出速度会随着定时中断时间而变化，进行限幅
int8 MOTOR = 1;
void Motor_Control(int16 Speed_l, int16 Speed_R)
{
    Encoder_GetData();
    motor_L.target_speed = Speed_l;
    motor_R.target_speed = Speed_R;
    if(MOTOR)
    {
        L = Motor_PID(&PID_Motor_L, (float)motor_L.encoder_speed, (float)motor_L.target_speed);
        R = Motor_PID(&PID_Motor_R, (float)motor_R.encoder_speed, (float)motor_R.target_speed);
    }   
    // Set_Motor_PWM(Motor_PID(&PID_Motor_L, (float)motor_L.encoder_speed, (float)motor_L.target_speed), Motor_PID(&PID_Motor_R, (float)motor_R.encoder_speed, (float)motor_R.target_speed));
    Set_Motor_PWM(L,R);
}


//简介：电机闭环差速控制
//参数：左右电机的目标速度，差速比例系数，差速限幅
//返回：无
//备注：放定时中断里面执行
//备注：输出速度会随着定时中断时间而变化，进行限幅
void Diff_Motor_Control(int16 Speed,float wai,float nei)
{
    Encoder_GetData();
    if(Err > 0)//要左转
    {
        motor_L.target_speed = Speed - nei * Err;//左轮减速
        motor_R.target_speed = Speed + wai * Err;//右轮加速

    }
    else if(Err < 0)//要右转
    {
        motor_L.target_speed = Speed + wai * Err;//左轮加速
        motor_R.target_speed = Speed - nei * Err;//右轮减速

    }
    else if(Err==0)
    {
        motor_L.target_speed = Speed;
        motor_R.target_speed = Speed;
    }


    L = Motor_PID(&PID_Motor_L, (float)motor_L.encoder_speed,(float)motor_L.target_speed);
    R = Motor_PID(&PID_Motor_R, (float)motor_R.encoder_speed, (float)motor_R.target_speed);
    Set_Motor_PWM(L,R);

    /* if(motor_L.encoder_speed<=50&&L>=2000)
    { 
        motor_L.target_speed=0;
    }
    if(motor_L.encoder_speed<=50&&L>=2000)
    {
        motor_R.target_speed = 0;
    } */
    // Set_Motor_PWM(Motor_PID(&PID_Motor_L, (float)motor_L.encoder_speed, (float)motor_L.target_speed), Motor_PID(&PID_Motor_R, (float)motor_R.encoder_speed, (float)motor_R.target_speed));
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
