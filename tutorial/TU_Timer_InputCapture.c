/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/

#include "ecInclude.h"

uint32_t ovf_cnt = 0;
uint32_t ccr_pre = 0;
uint32_t ccr_now = 0;
float    period = 0;

#define PA5 			5

int main(void){
	
	RCC_PLL_init(); 
	UART2_init();
  
	// GPIO configuration ---------------------------------------------------------------------
	GPIO_AF_set(GPIOA, PA5, NOPUPD, MSPEED, PUSHPULL);
	// TIM configuration -----------------------------------------------------------------------
	
	
//	uint8_t AFx = 0;
//	if ((TIMx == TIM1) || (TIMx == TIM2)) { AFx = 1UL;}
//	else if ((TIMx == TIM3) || (TIMx == TIM4) || (TIMx == TIM5)) { AFx = 2UL; }
//	else if ((TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11)) { AFx = 3UL; }
	

	GPIOA->AFR[PA5/8]	 &= ~(0xFUL	<< (4*(PA5%8)));			// 4 bit clear AFRx
	GPIOA->AFR[PA5/8]	 |= 	 1UL	<< (4*(PA5%8)); 
	
	RCC->APB1ENR |= 	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;       //TIM2 Clock enabled

	TIM2->PSC = 83;															   // Timer counter clock: 1M`Hz(1us)
	TIM2->ARR = 0xFFFF-1;														     // Set auto reload register to maximum (count up to 65535)
	TIM2->CR1 &= ~( 1 << 4); 										             // CR1 DIR enabled
	
	TIM2->CCMR1 &= ~(3 << 0 );	// clear
	TIM2->CCMR1 |= (1 << 0 );		// Capture/Compare Selection: CC1 is mapped on TI1 

	TIM2->CCMR1 &= ~(15 << 4 );                  // Clear IC1F
	TIM2->CCMR1 |=   (2 << 4);                   // Set filter N=4
	
	TIM2->CCER  &= ~(7 << 1);                    // Clear CCER
	TIM2->CCER  &= ~(7 << 1);	                   // Capture rising edge
	
	TIM2->CCER 	|= 	(1<<0);					             // Capture enabled
		
	TIM2->DIER |= 1 << 1;									       // Interrupt enabled
	TIM2->DIER |= 1 << 0;								         // Update interrupt enable	
	TIM2->CR1  |= 1 << 0;								         // Counter enable

	
	NVIC_SetPriority(TIM2, 2);						    	// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2);									   		// TIM2 interrupt request enable

	while(1)
		printf("period = %f[msec]\r\n", period);		// print out the period on TeraTerm
}

void TIM2_IRQHandler(void){
	
	if(TIM2->SR & TIM_SR_UIF){                    // Update interrupt
		//User code to handle overflow
		ovf_cnt++; // overflow를 세준다
		TIM2->SR &= ~(1 << 0);  							               	// clear update interrupt flag
	}
	
	if((TIM2->SR & TIM_SR_CC1IF) != 0){ 
		// User code to calculate the period of 1Hz pulse
		ccr_now = TIM2->CCR1;													// capture counter value
		period = ((ccr_now - ccr_pre) + (ovf_cnt)*(TIM2 -> ARR + 1)) / 1000; 						// calculate the period with ovf_cnt, ccr1, and ccr2
		// 1000을 나누는 이유는 마이크로 초를 밀리초로 바꾸기 위해
		ccr_pre = ccr_now;
		ovf_cnt = 0;
		
		TIM2->SR &= ~(1 << 0);		         // clear capture/compare interrupt flag ( it is also cleared by reading TIM2_CCR1)
	}
}
