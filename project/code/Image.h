#ifndef Image_H_
#define Image_H_

#include "zf_common_headfile.h"

extern uint8 mt9v03x_image_TwoValues[MT9V03X_H][MT9V03X_W];

void Image_Change_TwoValues(uint8 value);
void Image_LongestWhite_SearchLine();
void Image_Show_Boundry(void);
void Image_Cross_Detect();
int my_adapt_threshold(uint8 *image, uint16 col, uint16 row);
void Lost_Iamge();
void Zebra_Crossing();
void Straight_Detect();

extern int Cross_Flag;
extern int LostFlag;
extern int Zebra_Crossing_Flag;
extern int Straight_Flag; // 直道标志位
#endif // !Image_H_
