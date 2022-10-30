/**
******************************************************************************
* @author  
* @Mod		 2022-10-20 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  Midterm Exam
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
uint8_t cycle = 0;
uint8_t pause = 11;
uint32_t count=0;


int main(void) { 
	// Initialiization
	setup();
	GPIO_write(GPIOA, PA5, HIGH);
	
	// Inifinite Loop
	while(1){
		
		if(pause != 11){
			LED_toggle();	
		}			
		delay_ms(1000);	
		SysTick_reset();	
	}
}

// Initialiization 
void setup(void){
		
	RCC_PLL_init();	
	SysTick_init(1);
	TIM_INT_init(TIM2,500);
//	TIM_period_ms(TIM2, 500);
	// EXTI button setup
	EXTI_init(GPIOC, BUTTON_PIN,FALL,0);
	
	GPIO_out_set(GPIOA, PA5, PULLUP, MSPEED, PUSHPULL);
	
	// button setup
	GPIO_in_set(GPIOC, BUTTON_PIN, PULLUP);
	
	LED_4_init();
	sevensegment_init();
		
}



void EXTI15_10_IRQHandler(void){   // External Interrupt for pin 10 to 15
	if(is_pending_EXTI(BUTTON_PIN)){ // check the pending's flag on 
		
		sevensegment_decoder(pause);
		
//		bitToggle(TIMx->DIER , TIM_DIER_UIE);
		
		uint8_t swap = 0;
		swap = cycle;
		cycle =  pause; 
		pause = swap;
			
		TIM2->DIER ^= TIM_DIER_UIE;
		
		if(pause == 11){
			GPIO_write(GPIOA, PA5, HIGH);
		}
		
		
		clear_pending_EXTI(BUTTON_PIN); // clear the pending flag
	}
}


void TIM2_IRQHandler(void){
	if(is_pending_TIM(TIM2)){ // update interrupt flag
		
		 
		sevensegment_decoder(cycle);	
		LED_4_decoder(count_button);
			
		count_button ++;
		if(count_button > 15){
			count_button = 0;
			cycle++;
		}
		
		if(cycle > 9){
			cycle = 0;
		}
		
		clear_pending_TIM(TIM2);                     // clear by writing 0
	}
	
}



