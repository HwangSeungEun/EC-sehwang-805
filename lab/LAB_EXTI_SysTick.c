/**
******************************************************************************
* @author  
* @Mod		 2022-10-14 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  External Interrupt and SysTick Inturrput
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
		
			
		sevensegment_decoder(count_button);
		Delay(1000);	
		count_button ++; 
		if(count_button > 9){
			count_button = 0;
		}
		SysTick_reset();
		
	}
}



// Initialiization 
void setup(void){
	
	
	RCC_PLL_init();	
	SysTick_init(1);
	
	
	// EXTI button setup
	EXTI_init(GPIOC, BUTTON_PIN,FALL,0);
	
	// button setup
	GPIO_in_set(GPIOC, BUTTON_PIN, PULLUP);
			
	sevensegment_init();
		
}



void EXTI15_10_IRQHandler(void){   // External Interrupt for pin 10 to 15
	if(is_pending_EXTI(BUTTON_PIN)){ // check the pending's flag on 
		count_button = 0; 
		flag = count_button % 10;
		sevensegment_decoder(flag);
		SysTick_reset();
		clear_pending_EXTI(BUTTON_PIN); // clear the pending flag
	}
}

