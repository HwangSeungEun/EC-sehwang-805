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

#ifndef __ECFUNC_H
#define __ECFUNC_H

//#include "stm32f411xe.h"
//#include "ecRCC.h"
//#include "ecGPIO.h"



//Systick LED
#define PA5 		5

// 7segment
#define PB9 		9
#define PA6 		6
#define PA7 		7
#define PB6 		6
#define PC7 		7
#define PA9 		9
#define PA8 		8
#define PB10 		10



// 4LED
#define PA0			0
#define PA1			1
#define PB0			0
#define PC1			1


#define PB4			4
#define PB5			5
#define PB3			3
#define PA10		10


#define BUTTON_PIN 13

// bitwiser
void bitToggle(GPIO_TypeDef* Port, int pin);

// seven segment
void sevensegment_init(void);
void sevensegment_decoder(uint8_t flag);
void LED_toggle(void);

void LED_3_init(void);
void LED_3_decoder(uint8_t flag);

void LED_4_decoder(uint8_t flag);
void LED_4_init(void);


#ifdef __cplusplus

#endif /* __cplusplus */

#endif
