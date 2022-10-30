/**
******************************************************************************
* @author  
* @Mod		 2022-10-25 by SeungEun Hwang    	
* @brief   Embedded Controller:  LAB Timer PWM
* 
******************************************************************************
*/


#include "ecInclude.h"

#define PA1 		1

#define BUTTON_PIN 	13
#define LED_PIN 		5


void setup(void);
void EXTI15_10_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM3_IRQHandler(void);

int flag = 0;
uint32_t count=0;
PWM_t pwm;



int main(void) { 
	// Initialiization
	setup();
	
	// Inifinite Loop
	while(1){
		(flag == 0) ? 
		PWM_duty( &pwm, ((float)0.5 + ((float)count / 9))/(float)20 ) : 
		PWM_duty( &pwm, ((float)2.5 - ((float)count / 9))/(float)20 ); 	
	
	}	
}

// Initialiization 
void setup(void){
		
	RCC_PLL_init();	
	TIM_INT_init(TIM3,500);
	
	GPIO_in_set(GPIOC, BUTTON_PIN, PULLUP);
	EXTI_init(GPIOC, BUTTON_PIN,FALL,0);
	PWM_init(&pwm, GPIOA, PA1, FSPEED ,UPPCOUNT, 20);	
}

void TIM3_IRQHandler(void){
	if(is_pending_TIM(TIM3)){ // update interrupt flag	
		
		count++;		
		
		if(count > 18){
		
			count = 1;
			flag ^= 1;    // flag 
	
		}	
		clear_pending_TIM(TIM3);                     // clear by writing 0
	}
}


void EXTI15_10_IRQHandler(void){   // External Interrupt for pin 10 to 15
	if(is_pending_EXTI(BUTTON_PIN)){ // check the pending's flag on 
			
			reset_TIMER(TIM3);	
			flag = 0;
			count = 1;
		clear_pending_EXTI(BUTTON_PIN); // clear the pending flag
	}
}

