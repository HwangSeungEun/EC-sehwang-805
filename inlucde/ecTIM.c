/**
******************************************************************************
* @author  SeungEun Hwang
* @Mod		 2022-10-16 by SeungEun Hwang
* @brief   Embedded Controller:  EC_HAL_for_student_exercise 
* 
******************************************************************************
*/


#include "ecInclude.h"


/* Timer Configuration */

// 얘는 그냥 타이머를 초기 설정해주는 친구
// upper counter인지 down counter인지 parameter입력으로 정해주면 좋을 것 같다.
void TIM_init(TIM_TypeDef* TIMx, uint32_t msec){ 
	
// 1. Enable Timer CLOCK
	if			(TIMx ==TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if	(TIMx ==TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	
	else if	(TIMx ==TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if	(TIMx ==TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	else if	(TIMx ==TIM5) RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	else if	(TIMx ==TIM9) RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	else if	(TIMx ==TIM10) RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	else if	(TIMx ==TIM11) RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

	
// 2. Set CNT period
	TIM_period_ms(TIMx, msec); 
	
	
// 3. CNT Direction
	// Uppercounter	
	TIMx->CR1 &= ~(1 << 4);		// TIM_CR1_DIR_Msk
	// down counter 
//	TIMx->CR1 |= (1 << 4);					
	
// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;		
}


//	Q. Which combination of PSC and ARR for msec unit?
// 	Q. What are the possible range (in sec ?)
void TIM_period_us(TIM_TypeDef *TIMx, uint32_t usec){   
	// Period usec = 1 to 1000

	// 1us(1MHz, ARR = 1) to 65msec (ARR = 0xFFFF)
	
	// 1e-6
	if(TIMx == TIM2 || TIMx == TIM5){
		
		uint16_t PSCval = 84;					// 84/84000000 		= 1e-6  1MHz
		uint32_t ARRval = usec;  			// 1/1000000 			= 1e-6 = 1us  = 1MHz
		TIMx->PSC = PSCval - 1;
		TIMx->ARR =(ARRval - 1);					
	}else{
				
		uint16_t PSCval = 84;    // 84 000 00 
		uint16_t ARRval = (uint16_t)usec;  // 84/100000 
		
		TIMx->PSC = PSCval - 1;					
		TIMx->ARR =(ARRval - 1);		
	}		
}


void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec){ 
		
	// 0.1ms(10kHz, ARR = 1) to 6.5sec (ARR = 0xFFFF)
	
	if(TIMx == TIM2 || TIMx == TIM5){
		/*
		uint16_t PSCval = 84;					// 84/84000000 		= 1e-6  1MHz
		uint32_t ARRval = 1000;  			// 1000/1000000 		= 1e-3 = 1ms  1KHz
		TIMx->PSC = PSCval - 1;
		TIMx->ARR = (ARRval* msec - 1) ;
		*/
		// LAB에서 Timer Period를 100sec로 하기 위해서는 PSC를 8400 ARR을 10으로 해야한다. 반드시 그래야하는가?
		
//		uint16_t PSCval = 8400;					// 8400/84000000 		= 1e-4  10KHz
//		uint32_t ARRval = 10;  					// 10/10000 		= 1e-3 = 1ms  1KHz
		
		uint16_t PSCval = 840;					// 840/84000000 		= 1e-5  100KHz
		uint32_t ARRval = 100;  					// 100/100000 		= 1e-3 = 1ms  1KHz
		
		TIMx->PSC = PSCval - 1;
		TIMx->ARR = (ARRval* msec - 1) ;		
	}else {
		/*
		uint16_t PSCval = 84;					// 84/84000000 		= 1e-6  1MHz
		uint16_t ARRval = 1000;  			// 1000/1000000 		= 1e-3 = 1ms  1KHz
		TIMx->PSC = PSCval - 1;
		TIMx->ARR = (ARRval* msec - 1) ; 
		*/

//		uint16_t PSCval = 8400;					// 8400/84000000 		= 1e-4  1MHz
//		uint16_t ARRval = 10;  					// 10/10000 		= 1e-3 = 1ms  1KHz
		
		uint16_t PSCval = 840;					// 840/84000000 		= 1e-5  100KHz
		uint16_t ARRval = 100;  					// 100/100000 		= 1e-3 = 1ms  1KHz
		
		TIMx->PSC = PSCval - 1;
		TIMx->ARR = (ARRval* msec - 1) ; 
		
	}


	// 기본 설정 1ms에 msec를 곱해서 한주기에 걸리는 시간을 지정해줌
}


// 얘는 타이머 interrupt를 설정해서 타이머가 지나면 특정 행동을 하게 만드는 친구
// Update Event Interrupt
void TIM_INT_init(TIM_TypeDef* TIMx, uint32_t msec){
// 1. Initialize Timer	
	TIM_init(TIMx,msec);
	
// 2. Enable Update Interrupt
	TIM_INT_enable(TIMx);
	
// 3. NVIC Setting
	uint32_t IRQn_reg =0;
	if(TIMx == TIM1)       	IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if(TIMx == TIM2)  	IRQn_reg = TIM2_IRQn;
	else if(TIMx == TIM3)  	IRQn_reg = TIM3_IRQn;
	else if	(TIMx ==TIM4)		IRQn_reg = TIM4_IRQn;
	else if	(TIMx ==TIM5) 	IRQn_reg = TIM5_IRQn;
	// 9 10 11을 위한 IRQ가 따로 없어서 일단 TIM1과 연결된 값을 불러왔다. 
	else if	(TIMx ==TIM9)		IRQn_reg = TIM1_BRK_TIM9_IRQn;
	else if	(TIMx ==TIM10)	IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if	(TIMx ==TIM11)	IRQn_reg = TIM1_TRG_COM_TIM11_IRQn;
	// repeat for TIM3, TIM4, TIM5, TIM9, TIM10, TIM11

	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);                        
}



void TIM_INT_enable(TIM_TypeDef* TIMx){
	TIMx->DIER |= TIM_DIER_UIE;			// Enable Timer Update Interrupt		
}

void TIM_INT_disable(TIM_TypeDef* TIMx){
	TIMx->DIER &= ~TIM_DIER_UIE;				// Disable Timer Update Interrupt		
}

// update interrupt flag 
// pending
uint32_t is_UIF(TIM_TypeDef *TIMx){
	return ((TIMx->SR & TIM_SR_UIF) == TIM_SR_UIF);
}

void clear_UIF(TIM_TypeDef *TIMx){
	TIMx->SR &= ~TIM_SR_UIF;

}

void reset_TIMER(TIM_TypeDef* TIMx) {
	TIMx->CNT = 0;
}





