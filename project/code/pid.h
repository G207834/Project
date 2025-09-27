#ifndef __PID_H
#define __PID_H


typedef struct 
{   
    float Target;
     float Actual;
    // float Actual1;

    float Kp;       //P
    float Ki;       //I
    float Kd;       //D


    float  Out_p;  //KP输出
    float  Out_i;  //KI输出
    float  Out_d;  //KD输出
    float  Out;    //pid输出

    float Error;         //本次误差
    float Last_Error;   //上次误差
    float LL_Error;     //上上次误差                用于增量式PID
    float ErrorInt;     //误差积分

    /*输出限幅*/
    float OutMax;       
    float OutMin;

    /* 积分项限幅 */
    float ErrorIntMax;
    float ErrorIntMin;

} PID_t;

#define Pass   (0.9) //不完全微分滤波系数

float Servo_PD(PID_t *Pid,float get_error);
float Motor_PID(PID_t *Pid,float get_Actual,float target_Speed);



#endif