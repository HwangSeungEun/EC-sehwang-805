/*----------------------------------------------------------------\
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SeungEun Hwang
Created          : 09-13-2022
Modified         : 10-17-2022
Language/ver     : C++ in Keil uVision

Description      : ecInclude header file
/----------------------------------------------------------------*/

#ifndef __ECINCLUDE_H
#define __ECINCLUDE_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>


#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecRCC.h"
#include "ecGPIO.h"
#include "ecSysTick.h"
#include "ecFunc.h"
#include "ecEXTI.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecStepper.h"
#include "ecUART.h"
#include "ecIC.h"
#include "ecADC.h"
#include "ecRGBCS.h"


#define MCU_CLK_PLL 84000000
#define MCU_CLK_HSI 16000000
#define MCU_CLK_HSE 8000000



//#include "ecGPIO_API.h"



#ifdef __cplusplus

#endif /* __cplusplus */

#endif
