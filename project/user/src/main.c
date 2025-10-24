/*********************************************************************************************************************
 * CH32V307VCT6 Opensourec Library 即（CH32V307VCT6 开源库）是一个基于官方 SDK 接口的第三方开源库
 * Copyright (c) 2022 SEEKFREE 逐飞科技
 *
 * 本文件是CH32V307VCT6 开源库的一部分
 *
 * CH32V307VCT6 开源库 是免费软件
 * 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
 * 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
 *
 * 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
 * 甚至没有隐含的适销性或适合特定用途的保证
 * 更多细节请参见 GPL
 *
 * 您应该在收到本开源库的同时收到一份 GPL 的副本
 * 如果没有，请参阅<https://www.gnu.org/licenses/>
 *
 * 额外注明：
 * 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
 * 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
 * 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
 * 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
 *
 * 文件名称          main
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          MounRiver Studio V1.8.1
 * 适用平台          CH32V307VCT6
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期                                      作者                             备注
 * 2022-09-15        大W            first version
 ********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "Image.h"
#include "Island.h"
#include "motor.h"
#include "Control.h"
#include "motor.h"
#include "servo.h"
#include "pid.h"



// *************************** 例程硬件连接说明 ***************************
// 使用 type线 连接
//      模块管脚            单片机管脚
//      USB-DM      USB_DEVICE_CDC DM 默认 A11
//      USB-DP      USB_DEVICE_CDC DP 默认 A12

//
// 接入总钻风灰度数字摄像头 对应主板摄像头接口 请注意线序
//      模块管脚            单片机管脚
//      TXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_TX 宏定义 默认 D2  总钻风 UART-TX 引脚 要接在单片机 RX 上
//      RXD                 查看 zf_device_mt9v03x.h 中 MT9V03X_COF_UART_RX 宏定义 默认 C12 总钻风 UART-RX 引脚 要接在单片机 TX 上
//      D0                  查看 zf_device_mt9v03x.h 中 MT9V03X_D0_PIN      宏定义 默认 A9
//      D1                  查看 zf_device_mt9v03x.h 中 MT9V03X_D1_PIN      宏定义 默认 A10
//      D2                  查看 zf_device_mt9v03x.h 中 MT9V03X_D2_PIN      宏定义 默认 C8
//      D3                  查看 zf_device_mt9v03x.h 中 MT9V03X_D3_PIN      宏定义 默认 C9
//      D4                  查看 zf_device_mt9v03x.h 中 MT9V03X_D4_PIN      宏定义 默认 C11
//      D5                  查看 zf_device_mt9v03x.h 中 MT9V03X_D5_PIN      宏定义 默认 B6
//      D6                  查看 zf_device_mt9v03x.h 中 MT9V03X_D6_PIN      宏定义 默认 B8
//      D7                  查看 zf_device_mt9v03x.h 中 MT9V03X_D7_PIN      宏定义 默认 B9
//      PCLK                查看 zf_device_mt9v03x.h 中 MT9V03X_PCLK_PIN    宏定义 默认 A6
//      VSYNC               查看 zf_device_mt9v03x.h 中 MT9V03X_VSY_PIN     宏定义 默认 A5
//      HSYNC               查看 zf_device_mt9v03x.h 中 MT9V03X_HERF_PIN    宏定义 默认 A4

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底 摄像头接在主板的摄像头接口 注意线序
//
// 2.使用type-c线插入核心板的type-c接口
//
// 3.电脑上使用摄像头的传感器调试助手 串口波特率任意选，核心板按下复位按键
//
// 4.调试助手选择灰度图像 分辨率填写宽188 高120 勾选开始采集再打开对应的串口
//
// 5.上位机将会显示图像
//
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define LED1 (E9)

#define BEEP (C13)


 extern int Longest_WhiteLie_L[2];
extern int Longest_WhiteLie_R[2];

// 0：不包含边界信息
// 1：包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点
// 2：包含三条边线信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点，一般来说很少有这样的使用需求
// 3：包含三条边线信息，边界信息含有横纵轴坐标，意味着你可以指定每个点的横纵坐标，边线的数量也可以大于或者小于图像的高度，通常来说边线数量大于图像的高度，一般是搜线算法能找出回弯的情况
// 4：没有图像信息，仅包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点，这样的方式可以极大的降低传输的数据量
#define INCLUDE_BOUNDARY_TYPE 0

#define WIFI_SSID_TEST        "12345678"
#define WIFI_PASSWORD_TEST    "12345678"    // 如果需要连接的WIFI 没有密码则需要将 这里 替换为 NULL
#define TCP_TARGET_IP         "10.55.44.77" // 连接目标的 IP
#define TCP_TARGET_PORT       "8086"        // 连接目标的端口
#define WIFI_LOCAL_PORT       "6666"        // 本机的端口 0：随机  可设置范围2048-65535  默认 6666

// 边界的点数量远大于图像高度，便于保存回弯的情况
#define BOUNDARY_NUM (MT9V03X_H * 3 / 2)

// 只有X边界
uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];

// 只有Y边界
uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];

// X Y边界都是单独指定的
uint8 x1_boundary[MT9V03X_H], x2_boundary[MT9V03X_H], x3_boundary[MT9V03X_H];
uint8 y1_boundary[MT9V03X_W], y2_boundary[MT9V03X_W], y3_boundary[MT9V03X_W];

int main(void)
{

#if (0 != INCLUDE_BOUNDARY_TYPE)
    int32 i = 0;
#endif
#if (3 <= INCLUDE_BOUNDARY_TYPE)
    int32 j = 0;
#endif
    clock_init(SYSTEM_CLOCK_144M); // 初始化芯片时钟 工作频率为 144MHz
    debug_init();                  // 初始化默认 Debug UART

    /*  // 此处编写用户代码 例如外设初始化代码等
     while (wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST)) {
         printf("\r\n connect wifi failed. \r\n");
         system_delay_ms(100); // 初始化失败 等待 100ms
     }

     printf("\r\n module version:%s", wifi_spi_version);      // 模块固件版本
     printf("\r\n module mac    :%s", wifi_spi_mac_addr);     // 模块 MAC 信息
     printf("\r\n module ip     :%s", wifi_spi_ip_addr_port); // 模块 IP 地址

     // zf_device_wifi_spi.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接
     if (1 != WIFI_SPI_AUTO_CONNECT) // 如果没有开启自动连接 就需要手动连接目标 IP
     {
         while (wifi_spi_socket_connect( // 向指定目标 IP 的端口建立 TCP 连接
             "TCP",                      // 指定使用TCP方式通讯
             TCP_TARGET_IP,              // 指定远端的IP地址，填写上位机的IP地址
             TCP_TARGET_PORT,            // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
             WIFI_LOCAL_PORT))           // 指定本机的端口号
         {
             // 如果一直建立失败 考虑一下是不是没有接硬件复位
             printf("\r\n Connect TCP Servers error, try again.");
             system_delay_ms(100); // 建立连接失败 等待 100ms
         }
     }
  */
    /*  gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL); // 初始化 LED1 输出 默认高电平 推挽输出模式 */

    // 推荐先初始化摄像头，后初始化逐飞助手
    while (1) {
        if (mt9v03x_init())
            gpio_toggle_level(LED1); // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        else
            break;
        system_delay_ms(1000); // 闪灯表示异常
    }
    // 此处编写用户代码 例如外设初始化代码等

    // 逐飞助手初始化 数据传输使用USB CDC
    // seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);

    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_CUSTOM);

    //  如果要发送图像信息，则务必调用seekfree_assistant_camera_information_config函数进行必要的参数设置
    //  如果需要发送边线则还需调用seekfree_assistant_camera_boundary_config函数设置边线的信息

#if (0 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(仅包含原始图像信息)
    // seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image_TwoValues[0], MT9V03X_W, MT9V03X_H); // 配置发送二值化信息
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H); // 配置发送图像信息

#elif (1 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for (i = 0; i < MT9V03X_H; i++) {
        x1_boundary[i] = 70 - (70 - 20) * i / MT9V03X_H;
        x2_boundary[i] = MT9V03X_W / 2;
        x3_boundary[i] = 118 + (168 - 118) * i / MT9V03X_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, MT9V03X_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL, NULL);

#elif (2 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点)
    // 通常很少有这样的使用需求
    // 对边界数组写入数据
    for (i = 0; i < MT9V03X_W; i++) {
        y1_boundary[i] = i * MT9V03X_H / MT9V03X_W;
        y2_boundary[i] = MT9V03X_H / 2;
        y3_boundary[i] = (MT9V03X_W - i) * MT9V03X_H / MT9V03X_W;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, MT9V03X_W, NULL, NULL, NULL, y1_boundary, y2_boundary, y3_boundary);

#elif (3 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息含有横纵轴坐标)
    // 这样的方式可以实现对于有回弯的边界显示
    j = 0;
    for (i = MT9V03X_H - 1; i >= MT9V03X_H / 2; i--) {
        // 直线部分
        xy_x1_boundary[j] = 34;
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = 47;
        xy_y2_boundary[j] = i;

        xy_x3_boundary[j] = 60;
        xy_y3_boundary[j] = i;
        j++;
    }

    for (i = MT9V03X_H / 2 - 1; i >= 0; i--) {
        // 直线连接弯道部分
        xy_x1_boundary[j] = 34 + (MT9V03X_H / 2 - i) * (MT9V03X_W / 2 - 34) / (MT9V03X_H / 2);
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = 47 + (MT9V03X_H / 2 - i) * (MT9V03X_W / 2 - 47) / (MT9V03X_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;

        xy_x3_boundary[j] = 60 + (MT9V03X_H / 2 - i) * (MT9V03X_W / 2 - 60) / (MT9V03X_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }

    for (i = 0; i < MT9V03X_H / 2; i++) {
        // 回弯部分
        xy_x1_boundary[j] = MT9V03X_W / 2 + i * (138 - MT9V03X_W / 2) / (MT9V03X_H / 2);
        xy_y1_boundary[j] = i;

        xy_x2_boundary[j] = MT9V03X_W / 2 + i * (133 - MT9V03X_W / 2) / (MT9V03X_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;

        xy_x3_boundary[j] = MT9V03X_W / 2 + i * (128 - MT9V03X_W / 2) / (MT9V03X_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(XY_BOUNDARY, BOUNDARY_NUM, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);

#elif (4 == INCLUDE_BOUNDARY_TYPE)
    // 发送总钻风图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for (i = 0; i < MT9V03X_H; i++) {
        x1_boundary[i] = 70 - (70 - 20) * i / MT9V03X_H;
        x2_boundary[i] = MT9V03X_W / 2;
        x3_boundary[i] = 118 + (168 - 118) * i / MT9V03X_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, MT9V03X_W, MT9V03X_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, MT9V03X_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL, NULL);

#endif

    // USB虚拟串口初始化
    usb_cdc_init();

    // 波形图初始化
    /* seekfree_assistant_oscilloscope_struct oscilloscope_data; // 波形图初始化
    oscilloscope_data.data[0]     = 0;
    oscilloscope_data.channel_num = 1; */

    /* 屏幕初始化 */
    // ips200_set_dir(IPS200_CROSSWISE);
    // ips200_init(IPS200_TYPE_SPI);
    Init_ICM42688();

    // key_init(10);

    // uint8 Value = 163;
    uint8 Value;
    extern PID_t PID_Servo;
    // key_index_enum key_index_array[KEY_NUMBER] = {KEY_1,KEY_2,KEY_3,KEY_4};
    // key_init(5);
    // wireless_uart_init();
    //  Init_ICM42688();

Value = my_adapt_threshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    Encoder_Init();
    // Motor_Init();
    Servo_Init();
    Motor_Init();
    pit_ms_init(PIT_CH, 5);
    gpio_init(BEEP, GPO, GPIO_LOW, GPO_PUSH_PULL);
    extern int Island_State;
    // Set_Motor_PWM(1500,1500);
    // 左    占空比  编码器    右    占空比  编码器
    //       1000    230             1000    116
    //       1500    450             1500    200
    //       2000    660             2000    300
    /* uint8 state1 = 1;
    uint8 state2 = 1;
    uint8 state3 = 1;
    uint8 state4 = 1; */
    // Value = my_adapt_threshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    // 此处编写用户代码 例如外设初始化代码等
    while (1) {
        if (mt9v03x_finish_flag) {
            /* 二值化 */
            Image_Change_TwoValues(Value);
            mt9v03x_finish_flag = 0;
            if (Island_State != 3) {
                Image_LongestWhite_SearchLine();
                Lost_Iamge();
                // 1.9    0.002750   2.2  0.00283        2.25
                // 2.25      0.0029                      0.00295
                //  Set_Motor_PWM(1700 - 2.3 * (MT9V03X_H - Longest_WhiteLie_L[0]), 1700 - 2.3 * (MT9V03X_H - Longest_WhiteLie_L[0]));
                PID_Servo.Kp = 2.25 + 0.0045 * (MT9V03X_H - (Longest_WhiteLie_L[0] + Longest_WhiteLie_R[0]) / 2.0);
                Image_Cross_Detect();
            }
            Image_Island_Dect();
            Err_Sum();
            Servo_Control(Err);
            // printf("%f,%f,%f\n",GyroxInt,icm42688_gyro_y1,icm42688_gyro_z1);
            // printf("%f,%f,%f,%f,%f\n",Gkdout,Err,out,Pout,icm42688_gyro_x);
            // Dynamic_PD();

            //Image_Show_Boundry();
            //  /* 发送图像到串口 */
            //  // seekfree_assistant_camera_send();
            /* 屏幕显示总钻风，画幅和摄像头一致 */
            // ips200_displayimage03x(mt9v03x_image_TwoValues[0], 320, 200);
            // ips200_draw_line(160, 0, 160, 200, RGB565_RED);
            // ips200_show_int(0, 210, LostFlag, 1);
        }

        // switch(key_get_state(key_index_array[0]))
        //     {
        //         case KEY_SHORT_PRESS:
        //         MOTOR = 1;
        //         // PID_Motor_L.Ki +=0.1;
        //         break;
        //         case KEY_LONG_PRESS:
        //         if(state1)
        //         {
        //             // PID_Motor_L.Ki -=0.1;
        //             MOTOR = 1;
        //             state1=0;
        //         }
        //         key_clear_state(key_index_array[0]);
        //         break;
        //         case KEY_RELEASE:
        //         state1=1;
        //         break;
        //     }
        //     switch(key_get_state(key_index_array[1]))
        //     {
        //         case KEY_SHORT_PRESS:
        //         PID_Motor_L.Kp +=0.1;
        //         break;
        //         case KEY_LONG_PRESS:
        //         if(state2)
        //         {
        //             PID_Motor_L.Kp -=0.1;
        //             state2=0;
        //         }
        //         key_clear_state(key_index_array[1]);
        //         break;
        //         case KEY_RELEASE:
        //         state2=1;
        //         break;
        //     }
        //     switch(key_get_state(key_index_array[2]))
        //     {
        //         case KEY_SHORT_PRESS:
        //         PID_Motor_R.Ki +=0.1;
        //         break;
        //         case KEY_LONG_PRESS:
        //         if(state3)
        //         {
        //             PID_Motor_R.Ki -=0.1;
        //             state3=0;
        //         }
        //         key_clear_state(key_index_array[2]);
        //         break;
        //         case KEY_RELEASE:
        //         state3=1;
        //         break;
        //     }
        //     switch(key_get_state(key_index_array[3]))
        //     {
        //         case KEY_SHORT_PRESS:
        //         PID_Motor_R.Kp +=0.1;
        //         break;
        //         case KEY_LONG_PRESS:
        //         if(state4)
        //         {
        //             PID_Motor_R.Kp -=0.1;
        //             state4=0;
        //         }
        //         key_clear_state(key_index_array[3]);
        //         break;
        //         case KEY_RELEASE:
        //         state4=1;
        //         break;
        //     }

        // printf("ENCODER_l counter \t%d .\r\n", motor_L.encoder_raw);                 // 输出编码器计数信息
        // printf("ENCODER_r counter \t%d .\r\n", motor_R.encoder_raw);                 // 输出编码器计数信息
        // printf("i \t%d .\r\n", i);
        //  printf("\t%f .\r\n",icm42688_gyro_x);
        // printf("%d,%d,%d,%d\n",motor_L.encoder_speed,motor_L.target_speed,motor_R.encoder_speed,motor_R.target_speed);
        // Image_Show_Boundry();
        /* 发送图像到串口 */
        // seekfree_assistant_camera_send();
        /* 屏幕显示总钻风，画幅和摄像头一致 */
        // ips200_displayimage03x(mt9v03x_image_TwoValues[0], 320, 200);
        // ips200_draw_line(160, 0, 160, 200, RGB565_RED);

        /* seekfree_assistant_data_analysis();
        if (seekfree_assistant_parameter_update_flag[0]) {
            aaa = seekfree_assistant_parameter[0]; // 左边是单片机里要调整的变量，右边是通数据

            oscilloscope_data.data[0] = aaa;                          // 左边是波形通道，右边是要示波的参数
            seekfree_assistant_oscilloscope_send(&oscilloscope_data); // 发送波形
            seekfree_assistant_parameter_update_flag[0] = 0;          // 清除标志位
        } */
        // 此处编写需要循环执行的代码
    }
}

void pit_handler(void)
{
    // key_scanner();
    // gpio_toggle_level(LED1);
    // Encoder_GetData();
    // icm42688_gyro_transition();
    if (LostFlag) {
        Motor_Control(0, 0);
    } else {
        // Motor_Control(250, 250);
        // 1.5 2 1.2 2  0.8 2.3
        Diff_Motor_Control(350-(0.3*(MT9V03X_H - Longest_WhiteLie_R[0])), 0.55, 2.3);
    }
    // pid
    // WIFI波形图
    //  seekfree_assistant_data_analysis();
    //  if(seekfree_assistant_parameter_update_flag[0])
    //  {
    //      motor_L.encoder_speed=seekfree_assistant_parameter[0];//左边是单片机里要调整的变量，右边是通道接收的数据
    //      seekfree_assistant_oscilloscope_data.data[0] = motor_L.encoder_speed;//左边是波形通道，右边是要示波的参数
    //      seekfree_assistant_parameter_update_flag[0] = 0;//清除标志位

    // }

    // if(seekfree_assistant_parameter_update_flag[1])
    // {
    //     motor_L.target_speed=seekfree_assistant_parameter[1];//左边是单片机里要调整的变量，右边是通道接收的数据
    //     seekfree_assistant_oscilloscope_data.data[1] = motor_L.target_speed;//左边是波形通道，右边是要示波的参数
    //     seekfree_assistant_parameter_update_flag[1] = 0;//清除标志位

    // }

    // if(seekfree_assistant_parameter_update_flag[2])
    // {
    //     motor_R.encoder_speed=seekfree_assistant_parameter[2];//左边是单片机里要调整的变量，右边是通道接收的数据
    //     seekfree_assistant_oscilloscope_data.data[2] = motor_R.encoder_speed;//左边是波形通道，右边是要示波的参数
    //     seekfree_assistant_parameter_update_flag[2] = 0;//清除标志位

    // }

    // if(seekfree_assistant_parameter_update_flag[3])
    // {
    //     motor_L.target_speed=seekfree_assistant_parameter[3];//左边是单片机里要调整的变量，右边是通道接收的数据
    //     seekfree_assistant_oscilloscope_data.data[3] = motor_L.target_speed;//左边是波形通道，右边是要示波的参数
    //     seekfree_assistant_parameter_update_flag[3] = 0;//清除标志位

    // }

    // 舵机参数调试部分
    //  seekfree_assistant_data_analysis();
    //  if(seekfree_assistant_parameter_update_flag[0])
    //      {

    //         // Err=seekfree_assistant_parameter[0];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[0] = Err;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[0] = 0;//清除标志位

    //         // float tep = OutServo + 755;
    //         // tep = seekfree_assistant_parameter[1];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[1] = tep;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[1] = 0;//清除标志位

    //         // PID_Servo.Kp=seekfree_assistant_parameter[2];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[2] = PID_Servo.Kp;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[2] = 0;//清除标志位

    //         // PID_Servo.Kd=seekfree_assistant_parameter[3];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[3] = PID_Servo.Kd;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[3] = 0;//清除标志位

    //         // PID_Servo.Gd=seekfree_assistant_parameter[4];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[4] = PID_Servo.Gd;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[4] = 0;//清除标志位

    //         // //电机调参部分
    //         // motor_R.encoder_speed=seekfree_assistant_parameter[0];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[0] = motor_R.encoder_speed;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[0] = 0;//清除标志位

    //         // motor_R.target_speed= seekfree_assistant_parameter[1];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[1] = motor_R.target_speed;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[1] = 0;//清除标志位

    //         // PID_Motor_R.Kp=seekfree_assistant_parameter[2];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[2] = PID_Motor_R.Kp;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[2] = 0;//清除标志位

    //         // PID_Motor_R.Ki= seekfree_assistant_parameter[3];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[3] = PID_Motor_R.Ki;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[3] = 0;//清除标志位

    //         // PID_Motor_R.Kd=seekfree_assistant_parameter[4];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[4] = PID_Motor_R.Kd;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[4] = 0;//清除标志位

    //         // ERR=seekfree_assistant_parameter[5];//左边是单片机里要调整的变量，右边是通道接收的数据
    //         // seekfree_assistant_oscilloscope_data.data[5] = ERR;//左边是波形通道，右边是要示波的参数
    //         // seekfree_assistant_parameter_update_flag[5] = 0;//清除标志位

    //     }
}

/* void TIM6_IRQHandler(void)
{
    static int a = 0;
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
        a++;
        ips200_show_int(100, 210, a, 3);
        Set_Motor_PWM(10, 10);
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
} */
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
//
// 问题1：上位机没有图像
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口 或者 虚拟串口的COM号
//      如果是使用逐飞科技 WCH-LINK 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
//      如果串口和接线都正常 查看LED灯是否闪烁 如果闪烁证明初始化失败 检查摄像头接线是否正常 线序是否正确
//      如果摄像头接线都正常 LED灯不闪烁 联系技术客服即可
//
// 问题2：图像乱码、错位、不正常
//      检查是否选择的灰度图像选项
//      检查是否正确设置了图像宽度和高度 宽度和高度查看 zf_device_mt9v03x.h 中 MT9V03X_W 和 MT9V03X_H 宏定义
