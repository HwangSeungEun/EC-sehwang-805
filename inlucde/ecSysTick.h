/**
******************************************************************************
* @author  2022-10-15 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  SysTick Interrupt library
* 
******************************************************************************
*/

#ifndef __EC_SYSTICK_H
#define __EC_SYSTICK_H

//#include "stm32f4xx.h"
//#include "ecRCC.h"
//#include <stdint.h>

#include "ecInclude.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void SysTick_init(uint32_t msec);
void SysTick_Handler(void);
void SysTick_counter(void);
void delay_ms (uint32_t nTime);
void SysTick_reset(void);
uint32_t SysTick_val(void);

void SysTick_enable(void);
void SysTick_disable (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif