/**
******************************************************************************
* @author  2022-10-15 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  SysTick Interrupt library
* 
******************************************************************************
*/
#include "ecInclude.h"



volatile uint32_t TimeDelay;


// msec는 reload 값의 max를 정해줌으로써 전체 period의 시간을 정할 수 있는거다
void SysTick_ms_init(uint32_t msec){	

	//  SysTick Control and Status Register
	// Disable SysTick IRQ and SysTick Counter
	SysTick_disable();								

	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// SysTick Reload Value Register
	SysTick->LOAD = (MCU_CLK_PLL / (1000)) * msec - 1;						// 1ms, for HSI PLL = 84MHz.
	
	// SysTick Current Value Register
	SysTick_reset();

	// Enables SysTick exception request " 이거 이해가 잘 안간다"
	// 0 = Counting down to zero does not assert the SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
		
	// Enable SysTick IRQ and SysTick Timer
	SysTick_enable();
		
	NVIC_SetPriority(SysTick_IRQn, 16);		// Set Priority to 16
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC
}


void SysTick_us_init(uint32_t usec){	

	//  SysTick Control and Status Register
	// Disable SysTick IRQ and SysTick Counter
	SysTick_disable();								

	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// SysTick Reload Value Register
	SysTick->LOAD = (MCU_CLK_PLL / (1000000)) * usec - 1;						// 1ms, for HSI PLL = 84MHz.
	
	// SysTick Current Value Register
	SysTick_reset();

	// Enables SysTick exception request " 이거 이해가 잘 안간다"
	// 0 = Counting down to zero does not assert the SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
		
	// Enable SysTick IRQ and SysTick Timer
	SysTick_enable();
		
	NVIC_SetPriority(SysTick_IRQn, 16);		// Set Priority to 16
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC
}





void SysTick_Handler(void){
	SysTick_counter();	
}

void SysTick_counter(void){
	TimeDelay--;
}	


void delay_ms (uint32_t nTime){
	
	TimeDelay = nTime; //setup 
	while(TimeDelay != 0);	
}

void delay_us (uint32_t nTime){
	
	TimeDelay = nTime; //setup 
	while(TimeDelay != 0);	
}


	// SysTick -> VAL에서 VAL이 0이 되면 feedback loop에서 reload 값으로 다시 돌아간다
	// SysTick을 초기화시키는 방식 -> 다운 클락이기 때문에 가능한 방법이고 
void SysTick_reset(void)  {
	SysTick->VAL = 0;			// if VAL is 0, VAL will update Reroad value
}

uint32_t SysTick_val(void) {	// check the current VAL value
	return SysTick->VAL;
}

void SysTick_enable(void){		
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;		
}

void SysTick_disable(void){
	SysTick->CTRL = 0;	
}