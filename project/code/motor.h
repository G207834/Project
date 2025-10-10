#ifndef MOTOR_H
#define MOTOR_H

/*************************************电机定义部分***************************************************/

#define PWM_L      (TIM4_PWM_MAP1_CH2_D13)
#define DIR_L      (D12)

#define PWM_R      (TIM4_PWM_MAP1_CH3_D14)
#define DIR_R      (D15)

#define MotorLimit (0)

typedef struct
{

    int16 target_speed;
    int16 duty;
    int16 encoder_speed;
    int16 encoder_raw;
    // int32 tatal_encoder;

} motor;

extern motor motor_R;
extern motor motor_L;

void Motor_Init(void);
void Set_Motor_PWM(int32 motorL, int32 motorR);

/*************************************电机定义部分***************************************************/

/*************************************编码器定义部分***************************************************/

#define ENCODER_L   (TIM1_ENCOEDER)
#define ENCODER_L_A (TIM1_ENCOEDER_MAP3_CH1_E9)
#define ENCODER_L_B (TIM1_ENCOEDER_MAP3_CH2_E11)

#define ENCODER_R   (TIM10_ENCOEDER)
#define ENCODER_R_A (TIM10_ENCOEDER_MAP3_CH1_D1)
#define ENCODER_R_B (TIM10_ENCOEDER_MAP3_CH2_D3)

#define PIT         (TIM6_PIT)

#define X           (0.2) // 编码器低通滤波系数

void Encoder_Init(void);
void Encoder_GetData(void);
void Encoder_test(void);

/*************************************编码器定义部分***************************************************/

#endif