/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SSS LAB
Created          : 05-03-2021
Modified         : 09-29-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

#include "ecInclude.h"

#ifndef __EC_RCC_H
#define __EC_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//#include "stm32f411xe.h"

#define HSI 		0
#define HSE			1


void RCC_HSI_init(void);
void RCC_HSE_init(void);

void RCC_PLL_init(void);
void RCC_PLL_HSE_init(void);

void RCC_GPIOA_enable(void);
void RCC_GPIOB_enable(void);
void RCC_GPIOC_enable(void);
void RCC_GPIOD_enable(void);
void RCC_GPIOE_enable(void);
void RCC_GPIOH_enable(void);
void exam_RCC_PLL_init(int CLKSOURCE, int divM, int multN, int divP);

// void RCC_GPIO_enable(GPIO_TypeDef * GPIOx);

extern int EC_SYSCL;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
