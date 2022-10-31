/**
******************************************************************************
* @author  
* @Mod		 2022-10-25 by SeungEun Hwang    	
* @brief   Embedded Controller:  LAB Timer PWM
* 
******************************************************************************
*/


#include "ecInclude.h"

#define BUTTON_PIN		13

void setup(void);
void EXTI15_10_IRQHandler(void);

int flag  = 0;

int main(void) { 
	// Initialiization
	setup();

	// Inifinite Loop
	while(1){
		
		if(flag == 0){
			Stepper_step(FULLSTEP,CW, FULL, 2);
//			Stepper_step(HALFSTEP,CCW, HALF, 18);
		}
		else if (flag == 1){
			Stepper_stop();
		}
	}
	
	// Finding Max RPM
	// At FULL Mode :: Over 19, Motor Cannot Rotate Properly
	//Stepper_setSpeed(18, FULL);   // Max RPM to out Stepper Motor
	//Stepper_setSpeed(19, FULL);   // unacceptable RPM
	
	
	// At HALF Mode :: over 19, Motor Cannot Rotate Properly
	//Stepper_setSpeed(18, HALF);   // Max RPM to out Stepper Motor
	//Stepper_setSpeed(19, HALF);   // unacceptable RPM
	
}

// Initialiization 
void setup(void){
		
	RCC_PLL_init();	
	SysTick_us_init(1);
	
	GPIO_in_set(GPIOC, BUTTON_PIN, PULLUP);
	EXTI_init(GPIOC, BUTTON_PIN,FALL,0);
	
	// A = 1 = PB10 // A' = 2 = PB4 // B = 3 = PB5 // B' = 4 = PB3
	Stepper_init(GPIOB, PB10, GPIOB, PB4, GPIOB, PB5, GPIOB, PB3);
}

void EXTI15_10_IRQHandler(void){   // External Interrupt for pin 10 to 15
	if(is_pending_EXTI(BUTTON_PIN)){ // check the pending's flag on 
		
		Stepper_stop();		
		flag ^= 1;
		
		clear_pending_EXTI(BUTTON_PIN);
	}
}

