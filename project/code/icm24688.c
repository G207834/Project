 #include "zf_common_headfile.h"


//ICM42688加速度计数据
float icm42688_acc_x, icm42688_acc_y, icm42688_acc_z  ;       
// ICM42688角加速度数据
float icm42688_gyro_x, icm42688_gyro_y, icm42688_gyro_z ;    

// 静态函数声明,以下函数均为该.c文件内部调用
static void Write_Data_ICM42688(unsigned char reg, unsigned char dat);
static void Read_Datas_ICM42688(unsigned char reg, unsigned char *dat, unsigned int num);
// 数据转换为实际物理数据的转换系数
static float icm42688_acc_inv = 1, icm42688_gyro_inv = 1; 

/**
*
* @brief    ICM42688陀螺仪初始化
* @param
* @return   void
* @notes    用户调用
* Example:  Init_ICM42688();
*
**/
void Init_ICM42688(void)
{
    unsigned char time;
		unsigned char model; 

    spi_init(ICM42688_SPI, SPI_MODE0, ICM42688_SPI_SPEED, ICM42688_SPC_PIN, ICM42688_SDI_PIN, ICM42688_SDO_PIN, ICM42688_CS_PIN);
    gpio_init(ICM42688_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    
	// 读取陀螺仪型号陀螺仪自检
    model = 0xff;
    while(1)
    {
        time = 50;
        Read_Datas_ICM42688(ICM42688_WHO_AM_I, &model, 1);
        if(model == 0x47)
        {  
            // ICM42688
            break;
        }
        else
        {
            ICM42688_DELAY_MS(10);
            time--;
            if(time < 0)
            {
                while(1);
                // 卡在这里原因有以下几点
                // ICM42688坏了,如果是新的概率极低
                // 接线错误或者没有接好
            }
        }
    }
		Write_Data_ICM42688(ICM42688_PWR_MGMT0,0x00);				// 复位设备     
		ICM42688_DELAY_MS(10);															// 操作完PWR—MGMT0寄存器后200us内不能有任何读写寄存器的操作

    // 设置ICM42688加速度计和陀螺仪的量程和输出速率
    Set_LowpassFilter_Range_ICM42688(ICM42688_AFS_16G, ICM42688_AODR_32000HZ, ICM42688_GFS_2000DPS, ICM42688_GODR_32000HZ);

    Write_Data_ICM42688(ICM42688_PWR_MGMT0, 0x0F);      // 设置GYRO_MODE,ACCEL_MODE为低噪声模式
    ICM42688_DELAY_MS(10);

    // unsigned char dat[2];
    // Read_Datas_ICM42688(ICM42688_ACCEL_DATA_X1, dat, 2);
    // for(int i=0;i<100;i++)
    // {
    //     a += (2000.0f / 32768.0f * (short int)(((short int)dat[0] << 8) | dat[1])) * 0.005;

    // }
    // a = (2000.0f / 32768.0f * (short int)(((short int)dat[0] << 8) | dat[1])) * 0.005;

}

/**
*
* @brief    获得ICM42688陀螺仪加速度
* @param
* @return   void
* @notes    单位:g(m/s^2),用户调用
* Example:  Get_Acc_ICM42688();
*
**/
void Get_Acc_ICM42688(void)
{
    unsigned char dat[6];
    Read_Datas_ICM42688(ICM42688_ACCEL_DATA_X1, dat, 6);
    icm42688_acc_x = icm42688_acc_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    icm42688_acc_y = icm42688_acc_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    icm42688_acc_z = icm42688_acc_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}

/**
*
* @brief    获得ICM42688陀螺仪角加速度
* @param
* @return   void
* @notes    单位为:°/s,用户调用
* Example:  Get_Gyro_ICM42688();
*
**/
void Get_Gyro_ICM42688(void)
{
    unsigned char dat[6];
    Read_Datas_ICM42688(ICM42688_GYRO_DATA_X1, dat, 6);
    icm42688_gyro_x = icm42688_gyro_inv * (short int)(((short int)dat[0] << 8) | dat[1]);
    icm42688_gyro_y = icm42688_gyro_inv * (short int)(((short int)dat[2] << 8) | dat[3]);
    icm42688_gyro_z = icm42688_gyro_inv * (short int)(((short int)dat[4] << 8) | dat[5]);
}
float GyroxInt;
float a;
float icm42688_gyro_y1;
float icm42688_gyro_z1;

void icm42688_gyro_transition (void)
{
    unsigned char dat[6];
    Read_Datas_ICM42688(ICM42688_ACCEL_DATA_X1, dat, 6);
    // a = (2000.0f / 32768.0f * (short int)(((short int)dat[0] << 8) | dat[1])) * 0.005;
    GyroxInt += ((2000.0f / 32768.0f * (short int)(((short int)dat[0] << 8) | dat[1])) * 0.005) - 0.603933006;
    icm42688_gyro_y1 += ((2000.0f / 32768.0f * (short int)(((short int)dat[2] << 8) | dat[3])) * 0.005 )- 0.603933006;
    icm42688_gyro_z1 += ((2000.0f / 32768.0f * (short int)(((short int)dat[4] << 8) | dat[5])) * 0.005 )- 0.603933006;



    // float gyro_z_data = 0;
    // gyro_z_data = (float)icm42688_gyro_x / 16.4f; // 0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以 16.4      可以转化为带物理单位的数据，单位为：°/s
    // return gyro_z_data;
}





/**
*
* @brief    设置ICM42688陀螺仪低通滤波器带宽和量程
* @param    afs                 // 加速度计量程,可在dmx_icm42688.h文件里枚举定义中查看
* @param    aodr                // 加速度计输出速率,可在dmx_icm42688.h文件里枚举定义中查看
* @param    gfs                 // 陀螺仪量程,可在dmx_icm42688.h文件里枚举定义中查看
* @param    godr                // 陀螺仪输出速率,可在dmx_icm42688.h文件里枚举定义中查看
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Set_LowpassFilter_Range_ICM42688(ICM42688_AFS_16G,ICM42688_AODR_32000HZ,ICM42688_GFS_2000DPS,ICM42688_GODR_32000HZ);
*
**/
void Set_LowpassFilter_Range_ICM42688(enum icm42688_afs afs, enum icm42688_aodr aodr, enum icm42688_gfs gfs, enum icm42688_godr godr)
{
    Write_Data_ICM42688(ICM42688_ACCEL_CONFIG0, (afs << 5) | (aodr + 1));   // 初始化ACCEL量程和输出速率(p77)
    Write_Data_ICM42688(ICM42688_GYRO_CONFIG0, (gfs << 5) | (godr + 1));    // 初始化GYRO量程和输出速率(p76)

    switch(afs)
    {
    case ICM42688_AFS_2G:
        icm42688_acc_inv = 2000 / 32768.0f;             // 加速度计量程为:±2g
        break;
    case ICM42688_AFS_4G:
        icm42688_acc_inv = 4000 / 32768.0f;             // 加速度计量程为:±4g
        break;
    case ICM42688_AFS_8G:
        icm42688_acc_inv = 8000 / 32768.0f;             // 加速度计量程为:±8g
        break;
    case ICM42688_AFS_16G:
        icm42688_acc_inv = 16000 / 32768.0f;            // 加速度计量程为:±16g
        break;
    default:
        icm42688_acc_inv = 1;                           // 不转化为实际数据
        break;
    }
    switch(gfs)
    {
    case ICM42688_GFS_15_625DPS:
        icm42688_gyro_inv = 15.625f / 32768.0f;         // 陀螺仪量程为:±15.625dps
        break;
    case ICM42688_GFS_31_25DPS:
        icm42688_gyro_inv = 31.25f / 32768.0f;          // 陀螺仪量程为:±31.25dps
        break;
    case ICM42688_GFS_62_5DPS:
        icm42688_gyro_inv = 62.5f / 32768.0f;           // 陀螺仪量程为:±62.5dps
        break;
    case ICM42688_GFS_125DPS:
        icm42688_gyro_inv = 125.0f / 32768.0f;          // 陀螺仪量程为:±125dps
        break;
    case ICM42688_GFS_250DPS:
        icm42688_gyro_inv = 250.0f / 32768.0f;          // 陀螺仪量程为:±250dps
        break;
    case ICM42688_GFS_500DPS:
        icm42688_gyro_inv = 500.0f / 32768.0f;          // 陀螺仪量程为:±500dps
        break;
    case ICM42688_GFS_1000DPS:
        icm42688_gyro_inv = 1000.0f / 32768.0f;         // 陀螺仪量程为:±1000dps
        break;
    case ICM42688_GFS_2000DPS:
        icm42688_gyro_inv = 2000.0f / 32768.0f;         // 陀螺仪量程为:±2000dps
        break;
    default:
        icm42688_gyro_inv = 1;                          // 不转化为实际数据
        break;
    }
}              

/**
*
* @brief    ICM42688陀螺仪写8bit数据
* @param    data                数据
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Write_8bit_ICM42688(0x00);
*
**/
static void Write_8bit_ICM42688(unsigned char dat)  
{
    spi_write_8bit(ICM42688_SPI,dat);
}

/**
*
* @brief    ICM42688陀螺仪读8bit数据
* @param    data                数据
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Read_8bit_ICM42688(dat);
*
**/
static void Read_8bit_ICM42688(unsigned char *dat)
{
    *dat = spi_read_8bit(ICM42688_SPI);
}

/**
*
* @brief    ICM42688陀螺仪写数据
* @param    reg                 寄存器
* @param    data                需要写进该寄存器的数据
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Write_Data_ICM42688(0x00,0x00);
*
**/
static void Write_Data_ICM42688(unsigned char reg, unsigned char dat)
{
    ICM42688_CS(0);
    Write_8bit_ICM42688(reg);
    Write_8bit_ICM42688(dat);
    ICM42688_CS(1);
}

/**
*
* @brief    ICM42688陀螺仪读数据
* @param    reg                 寄存器
* @param    data                把读出的数据存入data
* @param    num                 数据个数
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Read_Datas_ICM42688(0x00,0x00,1);
*
**/
static void Read_Datas_ICM42688(unsigned char reg, unsigned char *dat, unsigned int num)
{
    ICM42688_CS(0);
		reg |= 0x80;
    Write_8bit_ICM42688(reg);
    while(num--)
        Read_8bit_ICM42688(dat++);
    ICM42688_CS(1);
}


