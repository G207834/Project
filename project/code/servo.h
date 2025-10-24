#ifndef Servo_H_
#define Servo_H_

#define Servo_PWM               (TIM2_PWM_MAP1_CH1_A15)                         // 定义主板上舵机对应引脚
#define Servo_FREQ              (50 )                                           // 定义主板上舵机频率  请务必注意范围 50-300

#define Servo_MID               (756)                                               //  舵机中值

#define Servo_L_MAX             (825)                                           // 定义主板上舵机活动范围 角度
#define Servo_R_MAX             (660)                                           // 定义主板上舵机活动范围 角度

#define Servo_DUTY(x)         ((float)PWM_DUTY_MAX / (1000.0 / (float)Servo_FREQ) * (0.5 + (float)(x) / 90.0))          //  舵机角度计算




void Servo_Init(void);
void Servo_Set(uint32 servo_motor_duty);
void Servo_Control(float error);
void Dynamic_PD(void);

#endif
