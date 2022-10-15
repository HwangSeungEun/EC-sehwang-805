/**
******************************************************************************
* @author  2022-10-15 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  External Interrupt library
* 
******************************************************************************
*/


#include "ecInclude.h"

void EXTI_init(GPIO_TypeDef *Port, int Pin, int trig_type, int priority){  

	// SYSCFG peripheral clock enable	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect External Line to the GPIO
	unsigned int EXTICR_port; 
	// 미리 지정되어 있는 포트의 일반화 번호들이다
	if			(Port == GPIOA) EXTICR_port = 0;
	else if	(Port == GPIOB) EXTICR_port = 1;
	else if	(Port == GPIOC) EXTICR_port = 2;
	else if	(Port == GPIOD) EXTICR_port = 3;
	else 										EXTICR_port = 4;
	
	/*	SYSCFG->EXTICR[BUTTON_PIN/4] &= ~SYSCFG_EXTICR4_EXTI13;  //~15<<(4* (pin%4))*/
	SYSCFG->EXTICR[Pin/4] &= ~( 15UL << (4*(Pin%4)) ) ;			// clear 4 bits
	SYSCFG->EXTICR[Pin/4] |= (EXTICR_port << (4*(Pin%4)) ) ;			// set 4 bits  무슨 포트인지 일반화
	
	// Configure Trigger edge
	if 			(trig_type == FALL) EXTI->FTSR |= (1UL << Pin);   // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= (1UL << Pin);   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable // 같은 pin에서 rising falling이 가능한가??
		EXTI->RTSR |= (1UL << Pin); 
		EXTI->FTSR |= (1UL << Pin);
	} 
	
	// Configure Interrupt Mask (Interrupt enabled)
	EXTI->IMR  |= (1UL << Pin);     // not masked, 왜 not masked 였지?
	
	
	// NVIC(IRQ) Setting
	uint8_t EXTI_IRQn = 0; 
	
	if (Pin < 5) 	EXTI_IRQn = Pin + 6;  					// EXTI0이 6번 핀을 가지고 있다. EXTI4 는 10번 핀에 할당되어 있다.
	else if	(Pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn; // 5~9번핀일 때  EXTI9_5_IRQn -> 23번핀에 할당
	else 			EXTI_IRQn = EXTI15_10_IRQn;  				// 10 ~ 15번 핀  40번 할당
								
	NVIC_SetPriority(EXTI_IRQn, priority);	// NVIC sets the order of execution according to prior
	NVIC_EnableIRQ(EXTI_IRQn); 	// EXTI IRQ enable
}



void EXTI_enable(uint32_t pin) {
	EXTI->IMR |= (1UL << pin);     // not masked (i.e., Interrupt enabled)
}
void EXTI_disable(uint32_t pin) {
	EXTI->IMR &= ~(1UL << pin);     // masked (i.e., Interrupt disabled)
}

uint32_t is_pending_EXTI(uint32_t pin){
	uint32_t EXTI_PRx = 1UL << pin;     	// check  EXTI pending 	
	return ((EXTI->PR & EXTI_PRx) == EXTI_PRx);
}


void clear_pending_EXTI(uint32_t pin){
	EXTI->PR |= 1UL << pin;     // clear EXTI pending 
}
