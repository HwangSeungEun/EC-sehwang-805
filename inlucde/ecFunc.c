/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Seung-Eun Hwang 
Created          : 05-03-2021
Modified         : 10-15-2022
Language/ver     : C in Keil uVision

Description      : Embedded Controller function 
/----------------------------------------------------------------*/


//#include "stm32f4xx.h"
//#include "stm32f411xe.h"
//#include "ecGPIO.h"
//#include "ecRCC.h"
//#include "ecFunc.h"

#include "ecInclude.h"


//sevensegment_decoder

void sevensegment_init(void) {

	// led a setup
	GPIO_out_set(GPIOA, PA8, NOPUPD, MSPEED, PUSHPULL);
	// led b setup
	GPIO_out_set(GPIOB, PB10, NOPUPD, MSPEED, PUSHPULL);
  // led c setup
	GPIO_out_set(GPIOA, PA7, NOPUPD, MSPEED, PUSHPULL);									
	// led d setup
	GPIO_out_set(GPIOA, PA6, NOPUPD, MSPEED, PUSHPULL);
	// led e setup
	GPIO_out_set(GPIOB, PB9, NOPUPD, MSPEED, PUSHPULL);
	// led f setup
	GPIO_out_set(GPIOA, PA9, NOPUPD, MSPEED, PUSHPULL);
	// led g setup
	GPIO_out_set(GPIOC, PC7, NOPUPD, MSPEED, PUSHPULL);
	// led DP setup
	GPIO_out_set(GPIOB, PB6, NOPUPD, MSPEED, PUSHPULL);	
	
}


void sevensegment_decoder(uint8_t flag) {

	int seven_segment[12][8] = {
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
				{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW},			//dot
				{LOW, LOW, HIGH, HIGH, LOW, LOW, LOW, HIGH}						// pause
				
	};

	GPIO_write(GPIOA, PA8, seven_segment[flag][0]); 		// a
	GPIO_write(GPIOB, PB10, seven_segment[flag][1]); 		// b
	GPIO_write(GPIOA, PA7, seven_segment[flag][2]); 		// c
	GPIO_write(GPIOA, PA6, seven_segment[flag][3]); 		// d
	GPIO_write(GPIOB, PB9, seven_segment[flag][4]); 		// e
	GPIO_write(GPIOA, PA9, seven_segment[flag][5]); 		// f
	GPIO_write(GPIOC, PC7, seven_segment[flag][6]); 		// g
	GPIO_write(GPIOB, PB6, seven_segment[flag][7]); 		// dp

}


void LED_4_init(void){
	
	// led 0bit setup
	GPIO_out_set(GPIOA, PA0, NOPUPD, MSPEED, PUSHPULL);
	// led 1bit setup
	GPIO_out_set(GPIOA, PA1, NOPUPD, MSPEED, PUSHPULL);
  // led 2bit setup
	GPIO_out_set(GPIOB, PB0, NOPUPD, MSPEED, PUSHPULL);									
	// led 3bit setup
	GPIO_out_set(GPIOC, PC1, NOPUPD, MSPEED, PUSHPULL);
	
	uint8_t flag_init = 0;
	
	LED_4_decoder(flag_init);
}

void LED_4_decoder(uint8_t flag){
	
	int led4[16][4] = { 
		
		{0,0,0,0},		// 0
		{0,0,0,1},		// 1
		{0,0,1,0},		// 2
		{0,0,1,1},		// 3
		{0,1,0,0},		// 4
		{0,1,0,1},		// 5
		{0,1,1,0},		// 6
		{0,1,1,1},		// 7
		{1,0,0,0},		// 8
		{1,0,0,1},		// 9
		{1,0,1,0},		// 10
		{1,0,1,1},		// 11
		{1,1,0,0},		// 12
		{1,1,0,1},		// 13
		{1,1,1,0},		// 14
		{1,1,1,1}			// 15			
	};
	
	GPIO_write(GPIOA, PA0, led4[flag][0]); 		// a
	GPIO_write(GPIOA, PA1, led4[flag][1]); 		// b
	GPIO_write(GPIOB, PB0, led4[flag][2]); 		// c
	GPIO_write(GPIOC, PC1, led4[flag][3]); 		// d
	
}

void LED_3_init(void){
	
	// led 0bit setup
	GPIO_out_set(GPIOA, PA0, NOPUPD, MSPEED, PUSHPULL);
	// led 1bit setup
	GPIO_out_set(GPIOA, PA1, NOPUPD, MSPEED, PUSHPULL);
  // led 2bit setup
	GPIO_out_set(GPIOB, PB0, NOPUPD, MSPEED, PUSHPULL);									
	
	uint8_t flag_init = 0;
	
	LED_3_decoder(flag_init);
}

void LED_3_decoder(uint8_t flag){
	
	int led3[8][3] = { 
		
		{0,0,0},		// 0
		{0,0,1},		// 1
		{0,1,0},		// 2
		{0,1,1},		// 3
		{1,0,0},		// 4
		{1,0,1},		// 5
		{1,1,0},		// 6
		{1,1,1},		// 7		
	};
	
	GPIO_write(GPIOA, PA0, led3[flag][0]); 		// a
	GPIO_write(GPIOA, PA1, led3[flag][1]); 		// b
	GPIO_write(GPIOB, PB0, led3[flag][2]); 		// c
	
}








void bitToggle(GPIO_TypeDef* Port, int pin) {

	Port->ODR ^= (1 << pin);

}

void LED_toggle(void){
	
	GPIOA->ODR ^= (1 << 5);
	
}

void LED4_toggle(uint8_t flag){ 	// use truth table of 4 leds circuit
		
	int led4[4][4] = { 
		{HIGH,		LOW,		LOW,		LOW},
		{LOW,		HIGH,		LOW,		LOW},
		{LOW,		LOW,		HIGH,		LOW},
		{LOW,		LOW,		LOW,  		HIGH}
	};
	
		GPIO_write(GPIOA, PA5, led4[flag][0]); 		// PA5
		GPIO_write(GPIOA, PA6, led4[flag][1]); 		// PA6
		GPIO_write(GPIOA, PA7, led4[flag][2]); 		// PA7
		GPIO_write(GPIOB, PB6, led4[flag][3]); 		// PB6d
}


