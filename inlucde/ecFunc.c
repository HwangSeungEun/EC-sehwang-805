/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SSS LAB
Created          : 05-03-2021
Modified         : 09-29-2022
Language/ver     : C++ in Keil uVision

Description      : Embedded Controller function 
/----------------------------------------------------------------*/


//#include "stm32f4xx.h"
//#include "stm32f411xe.h"
//#include "ecGPIO.h"
//#include "ecRCC.h"
//#include "ecFunc.h"

#include "ecInclude.h"



void Seven_Segment_init(void) {

	// led a setup
	GPIO_init(GPIOA, PA8, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOA, PA8, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOA, PA8, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOA, PA8, MSPEED);		// LED Medium speed
	GPIO_write(GPIOA, PA8, LOW); 		// claer LED	

	// led b setup
	GPIO_init(GPIOB, PB10, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOB, PB10, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOB, PB10, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOB, PB10, MSPEED);		// LED Medium speed
	GPIO_write(GPIOB, PB10, LOW); 		// claer LED

	// led c setup
	GPIO_init(GPIOA, PA7, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOA, PA7, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOA, PA7, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOA, PA7, MSPEED);		// LED Medium speed
	GPIO_write(GPIOA, PA7, LOW); 		// claer LED	

	// led d setup
	GPIO_init(GPIOA, PA6, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOA, PA6, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOA, PA6, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOA, PA6, MSPEED);		// LED Medium speed
	GPIO_write(GPIOA, PA6, LOW); 		// claer LED	

	// led e setup
	GPIO_init(GPIOA, PA5, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOA, PA5, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOA, PA5, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOA, PA5, MSPEED);		// LED Medium speed
	GPIO_write(GPIOA, PA5, LOW); 		// claer LED

	// led f setup
	GPIO_init(GPIOA, PA9, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOA, PA9, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOA, PA9, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOA, PA9, MSPEED);		// LED Medium speed
	GPIO_write(GPIOA, PA9, LOW); 		// claer LED	

	// led g setup
	GPIO_init(GPIOC, PC7, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOC, PC7, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOC, PC7, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOC, PC7, MSPEED);		// LED Medium speed
	GPIO_write(GPIOC, PC7, HIGH); 		// claer LED

	// led DP setup
	GPIO_init(GPIOB, PB6, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype(GPIOB, PB6, PUSHPULL);	// LED open drain
	GPIO_pupd(GPIOB, PB6, NOPUPD);		// LED NOPUPD 
	GPIO_ospeed(GPIOB, PB6, MSPEED);		// LED Medium speed
	GPIO_write(GPIOB, PB6, HIGH); 		// claer LED

}


void Seven_segment_decoder(int flag) {

	int seven_segment[11][8] = {
				{LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH},          //zero
				{HIGH,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH},          //one
				{LOW,	LOW,	HIGH,	LOW,	LOW,	HIGH,	LOW,	HIGH},          //two
				{LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	LOW,	HIGH},          //three
				{HIGH,	LOW,	LOW,	HIGH,	HIGH,	LOW,	LOW,	HIGH},          //four
				{LOW,	HIGH,	LOW,	LOW,	HIGH,	LOW,	LOW,	HIGH},          //five
				{LOW,	HIGH,	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH},          //six
				{LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH},          //seven
				{LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH},          //eight
				{LOW,	LOW,	LOW,	HIGH,	HIGH,	LOW,	LOW,	HIGH},          //nine
				{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW}          //dot
	};


	GPIO_write(GPIOA, PA8, seven_segment[flag][0]); 		// a
	GPIO_write(GPIOB, PB10, seven_segment[flag][1]); 		// b
	GPIO_write(GPIOA, PA7, seven_segment[flag][2]); 		// c
	GPIO_write(GPIOA, PA6, seven_segment[flag][3]); 		// d
	GPIO_write(GPIOA, PA5, seven_segment[flag][4]); 		// e
	GPIO_write(GPIOA, PA9, seven_segment[flag][5]); 		// f
	GPIO_write(GPIOC, PC7, seven_segment[flag][6]); 		// g
	GPIO_write(GPIOB, PB6, seven_segment[flag][7]); 		// dp



}


void bitToggle(GPIO_TypeDef* Port, int pin) {

	Port->ODR ^= (1 << pin);

}