#ifndef __MY_RGCCS_H
#define __MY_RGCCS_H

#include "ecInclude.h"


#define S0_PORT 		GPIOC
#define S0_PIN 			9
#define S1_PORT 		GPIOB
#define S1_PIN  		8
#define S2_PORT 		GPIOC
#define S2_PIN  		8
#define S3_PORT 		GPIOC
#define S3_PIN  		6
#define CS_EN_PORT 	GPIOC
#define CS_EN_PIN		5
#define RGB1_PORT 	GPIOA
#define RGB1_PIN 		0
#define RGB2_PORT 	GPIOA
#define RGB2_PIN 		6
#define RGB3_PORT 	GPIOB
#define RGB3_PIN 		6

#define RGB_PWRDWN	0
#define RGB_2_PER		1
#define RGB_20_PER	2
#define RGB_100_PER	3

void RGBCS_init(void);
void RGBCS_freqScale(int mode);
void RGBCS_IC_config(void);
void RGBCS_Red_config(void);
void RGBCS_Green_config(void);
void RGBCS_Blue_config(void);
#endif
