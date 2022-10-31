/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
//#include "ecInclude.h"
//#include "ecTIM.h"

uint32_t count=0;



uint32_t count1=0;

uint32_t count2=0;

#define LED_PIN 	5

void setup(void);
void TIM2_IRQHandler(void);

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
//	TIM_TypeDef* timerx;
//	timerx = TIM2;
//	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
//	
//	timerx->PSC = 84 -1;							        	// (84-1) prescaler PLL divider --> Timer counter clock: 1MHz(1us)
//	timerx->ARR = 1000 - 1;									// reroad 값이 여기서 업데이트 된다 Set auto reload register to maximum (count up to 65535)
//	timerx->DIER |= 1 << 0;                   // Enable Interrupt
//	timerx->CR1 &= ~TIM_CR1_DIR;							// 0:upper counter 1:downcounter
//	timerx->CR1 |=  1 << 0;                  // 0: counter disable 1: counter Enable 
//	
//	NVIC_EnableIRQ(TIM2_IRQn);				            // TIM2_IRQHandler Enable
//	NVIC_SetPriority( TIM2_IRQn,2);               // TIM2_IRQHandler Set priority as 2
//	
	// Inifinite Loop ----------------------------------------------------------
	while(1){}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                       // System Clock = 84MHz
//	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_out_set(GPIOB, 4, PULLUP, MSPEED, PUSHPULL);
	GPIO_out_set(GPIOB, 5, PULLUP, MSPEED, PUSHPULL);
	TIM_INT_init(TIM2,1);
}

//void TIM2_IRQHandler(void){
//	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ // update interrupt flag
//		// 2초 깜빡일 때 나머지는 한번 깜빡
//		count1 ++ ;
//		count2 ++ ;
//		
//		if(count1 > 1000){

//			bitToggle(GPIOB, 4);
//			count1 = 0;	
//		}
//		
//		
//		if(count2 > 2000){
//				bitToggle(GPIOB, 5);
//				count2 = 0;	
//		}
//		
//		
//		//Create the code to toggle LED by 1000ms
//		TIM2->SR &= ~TIM_SR_UIF ;                       // clear by writing 0
//	}
//	
//}


void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ // update interrupt flag
		// 2초 깜빡일 때 나머지는 한번 깜빡

		count ++ ;
		// 1000으로 나누면 나머지가 999되고 1000되면 몫이 1되면서 나머지가 0 된다
		if((count % 1000) == 999){

			bitToggle(GPIOB, 4);

		}
		
		
		if(count > 2000){
				bitToggle(GPIOB, 5);
				count = 0;	
		}
		
		
		//Create the code to toggle LED by 1000ms
		TIM2->SR &= ~TIM_SR_UIF ;                       // clear by writing 0
	}
	
}

