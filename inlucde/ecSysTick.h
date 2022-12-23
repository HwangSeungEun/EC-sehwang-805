/**
******************************************************************************
* @author  2022-10-15 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  SysTick Interrupt library
* 
******************************************************************************
*/
#include "ecInclude.h"

#ifndef __EC_SYSTICK_H
#define __EC_SYSTICK_H

//#include "stm32f4xx.h"
//#include "ecRCC.h"
//#include <stdint.h>



#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void SysTick_ms_init(uint32_t msec);
void SysTick_us_init(uint32_t usec);
void SysTick_Handler(void);
void SysTick_counter(void);
void delay_ms (uint32_t nTime);
void delay_us (uint32_t nTime);
void SysTick_reset(void);
uint32_t SysTick_val(void);

void SysTick_enable(void);
void SysTick_disable (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif