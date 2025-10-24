#ifndef MOTOR_H
#define MOTOR_H

#include "pid.h"

/*************************************电机定义部分***************************************************/

#define PWM_L      (TIM4_PWM_MAP1_CH2_D13)
#define DIR_L      (D12)

#define PWM_R      (TIM4_PWM_MAP1_CH3_D14)
#define DIR_R      (D15)

#define MotorLimit (0)

//调试时所需参数
extern float L;
extern float R;


typedef struct
{

    int16 target_speed;
    int16 encoder_speed;
    int16 encoder_raw;
    // int32 tatal_encoder;

} motor;

extern PID_t PID_Motor_L;
extern PID_t PID_Motor_R;

extern motor motor_R;
extern motor motor_L;

extern float L;
extern float R;
extern int8 MOTOR;

void Motor_Init(void);
void Set_Motor_PWM(int32 motorL, int32 motorR);
void Motor_Control(int16 Speed_l, int16 Speed_R);
// void Diff_Motor_Control(int16 Speed,float k);
// void Diff_Motor_Control(int16 Speed,float kl,float kr);
void Diff_Motor_Control(int16 Speed,float wai,float nei);


/*************************************电机定义部分***************************************************/

/*************************************编码器定义部分***************************************************/


#define ENCODER_L   (TIM3_ENCOEDER)
#define ENCODER_L_A (TIM3_ENCOEDER_MAP3_CH1_C6)
#define ENCODER_L_B (TIM3_ENCOEDER_MAP3_CH2_C7)



#define ENCODER_R   (TIM9_ENCOEDER)
#define ENCODER_R_A (TIM9_ENCOEDER_MAP3_CH1_D9)
#define ENCODER_R_B (TIM9_ENCOEDER_MAP3_CH2_D11)

#define PIT         (TIM6_PIT)

#define X           (0.15) // 编码器低通滤波系数

void Encoder_Init(void);
void Encoder_GetData(void);
void Encoder_test(void);

/*************************************编码器定义部分***************************************************/


#define PIT_CH                  (TIM6_PIT )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (TIM6_IRQn)  


#endif