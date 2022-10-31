/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SSS LAB
Created          : 05-03-2021
Modified         : 10-14-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/


//#include "stm32f411xe.h"
//#include "ecRCC.h"

#include "ecInclude.h"

#ifndef __ECGPIO_H
#define __ECGPIO_H

// GPIO Mode
#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

#define HIGH 1
#define LOW  0

// GPIO Speed          : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
#define LSPEED  0x00
#define MSPEED  0x01
#define FSPEED  0x02
#define HSPEED  0x03

// Output type
#define PUSHPULL  	0
#define OPDRAIN  		1

// Pullup Pulldown
// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
#define NOPUPD  0x00
#define PULLUP  0x01
#define PULLDO  0x02
#define RESERV  0x03





#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void setup(void);	 
	 
	 
void GPIO_in_set(GPIO_TypeDef *Port, int pin, int pupd);	 
void GPIO_out_set(GPIO_TypeDef *Port, int pin, int pupd, int speed, int type);	 
void GPIO_AF_set(GPIO_TypeDef *Port, int pin, int pupd, int speed, int type);


void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
void GPIO_write(GPIO_TypeDef *Port, int pin, int Output);
int  GPIO_read(GPIO_TypeDef *Port, int pin);
void GPIO_mode(GPIO_TypeDef* Port, int pin, int mode);
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed);
void GPIO_otype(GPIO_TypeDef* Port, int pin, int type);
void GPIO_pupd(GPIO_TypeDef* Port, int pin, int pupd);

 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
