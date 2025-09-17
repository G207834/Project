#include "Image.h"

uint8 mt9v03x_image_TwoValues[MT9V03X_H][MT9V03X_W];

/* 图像二值化，固定阈值，原图0-255>黑-白,二值图0-1>黑—白*/
void Image_Change_TwoValues(uint8 Value)
{
    uint8 Temp;
    for (uint8 i = 0; i < MT9V03X_H; i++) 
    {
        for (uint8 j = 0; j < MT9V03X_W; j++)
        {
            Temp = mt9v03x_image[i][j];//中间变量似乎非必要
            if(Temp<Value)
            {
                mt9v03x_image_TwoValues[i][j] = 0;
            }
            else
            {
                mt9v03x_image_TwoValues[i][j] = 0;
            }
        }
    }
}

    /* 动态阈值，大津法 */