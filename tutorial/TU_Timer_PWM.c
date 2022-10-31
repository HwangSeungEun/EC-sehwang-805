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


#define LED_PIN 	5

void setup(void);
	
int count = 0;	
PWM_t *LED_PWM;
	
int main(void) { 
	// Initialiization --------------------------------------------------------

	
	
	
//	GPIO_init(GPIOA, LED_PIN, ______);     // GPIOA 5 ALTERNATE function
//	GPIO_ospeed(GPIOA, LED_PIN, ______);   // GPIOA 5 HIGH SPEED
	
	// TEMP: TIMER Register Initialiization --------------------------------------------------------		
//	TIM_TypeDef *TIMx;
//	TIMx = TIM2;
	
	
//	GPIOA->AFR[0]	 &= ~(15 << (4*LED_PIN));
//	GPIOA->AFR[0]	 |= 1 << (4*LED_PIN);          						// AF1 at PA5 = TIM2_CH1 (p.150)  타이머마다 AF값이 다르다 
	
	// TIMER: PWM setting
	
	
	
	
//	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	  // Enable TIMER clock
//	
//	TIMx->CR1 &= ~(1 << 4);								// TIM_CR1_DIR_Msk		// Direction Up-count
//	
//	uint16_t PSCval = 840UL;
//	
//	TIMx->PSC = 	PSCval - 1;  // Set Timer CLK = 100kHz : (PSC + 1) = 84MHz/100kHz --> PSC = ?
//	
//	uint16_t ARRval = 100;
//	
//	TIMx->ARR = ARRval - 1;    // Auto-reload: Upcounting (0...ARR). 
//																				// Set Counter CLK = 1kHz : (ARR + 1) = 100kHz/1kHz --> ARR = ?

//	
//	// CH1, CH2에 대한것 CCMR1에서 채널 1,2 			
//	// CH3,4는 CCMR2에서 다룬다
//	TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;  			// Clear ouput compare mode bits for channel 1
//	TIMx->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;                			// OC1M = 110 for PWM Mode 1 output on ch1
//	TIMx->CCMR1	|= TIM_CCMR1_OC1PE;    		// Output 1 preload enable (make CCR1 value changable)
//	
//	TIMx->CCR1 =  (TIMx->ARR + 1)/2;   // 100/2 = 49   	0~49 는 50개 									// Output Compare Register for channel 1 	
//	
//	TIMx->CCER &= ~TIM_CCER_CC1P;    			// select output polarity: active high	
//	TIMx->CCER |= TIM_CCER_CC1E;					// Enable output for ch1
//	
//	TIMx->CR1  |= TIM_CR1_CEN;      			// Enable counter
	
	// Inifinite Loop ----------------------------------------------------------
//	while(1){
//			for(int i = 0; i < 3; i++){
//				LED_PWM->timer->CCR1 = LED_PWM->timer->ARR * i / 2;
//				delay_ms(1000);
//			} 
//		}


		while(1)
		{;}
	}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();       // System Clock = 84MHz
	TIM_INT_init(TIM3,500);
	PWM_init(LED_PWM, GPIOA, LED_PIN, UPPCOUNT); // PA5 --> TIM2, ch1
}


void TIM3_IRQHandler(void){
	if((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF){ // update interrupt flag
		
		LED_PWM -> timer -> CCR1 = LED_PWM->timer->ARR * count / 2;	
		count ++;
		if(count > 3){
			count = 0;
		}
		
		TIM3->SR &= ~TIM_SR_UIF ;                       // clear by writing 0
	}
	
}


