/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-10-14 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  Tutorial Digital In
*					 Toggle Multi LED
* 
******************************************************************************
*/


#include "ecInclude.h"

#define PA5 		5
#define PA6 		6
#define PA7 		7
#define PB6 		6

#define BUTTON_PIN 13

void setup(void);
void EXTI15_10_IRQHandler(void);

int count_button = 0;
uint8_t flag = 0;

int main(void) { 
	
	// Initialiization
	setup();

	// Inifinite Loop
	while(1){
	Delay(150);	
	}
}

// Initialiization 
void setup(void){
	
	RCC_PLL_init();	
	SysTick_init(1);
	
	// EXTI button setup
	EXTI_init(GPIOC, BUTTON_PIN,FALL,0);
	
//	// button setup
//	GPIO_in_set(GPIOC, BUTTON_PIN, PULLUP);
		
	// led 1 setup
	GPIO_out_set(GPIOA, PA5, PULLUP, MSPEED, PUSHPULL);
	
	// led 2 setup
	GPIO_out_set(GPIOA, PA6, PULLUP, MSPEED, PUSHPULL);
	
	// led 3 setup
	GPIO_out_set(GPIOA, PA7, PULLUP, MSPEED, PUSHPULL);
										
	// led 4 setup
	GPIO_out_set(GPIOB, PB6, PULLUP, MSPEED, PUSHPULL);
		
}

void EXTI15_10_IRQHandler(void){
	if(is_pending_EXTI(BUTTON_PIN)){
		count_button ++; 
		flag = count_button % 4;
		LED4_toggle(flag);
		
//		clear_pending_EXTI(BUTTON_PIN);
	}
}

