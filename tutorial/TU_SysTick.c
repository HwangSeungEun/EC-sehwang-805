//#include "stm32f411xe.h"
//#include "ecRCC.h"
//#include "ecGPIO.h"
#include "ecInclude.h"

#define MCU_CLK_PLL 84000000
#define MCU_CLK_HSI 16000000
#define LED_PIN 5

volatile uint32_t msTicks = 0;
volatile uint32_t curTicks;

void setup(void);

int main(void) {
	
// System CLOCK, GPIO Initialiization ----------------------------------------
	setup();

// SysTick Initialiization ------------------------------------------------------				
	//  SysTick Control and Status Register
	SysTick-> CTRL = 0;				// Disable SysTick IRQ and SysTick Counter  --> 전체를 다 초기화 한다.

	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= 1 << 2;

	// uint32_t MCU_CLK=EC_SYSTEM_CLK
	// SysTick Reload Value Register
	SysTick->LOAD = (MCU_CLK_PLL/1000)-1; 				// 1ms

	// Clear SysTick Current Value 
	SysTick->VAL = 0;

	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= 1 << 1;
		
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= 1 << 0;
		
	NVIC_SetPriority(SysTick_IRQn, 16);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC

		
// While loop ------------------------------------------------------				
	msTicks = 0;

	while(1){
		curTicks = msTicks;
		while ((msTicks - curTicks) < 1000);	// delay fuction
		msTicks = 0;
		LED_toggle();
	}
}

void SysTick_Handler(void){
	msTicks++;
}

void setup(void)
{
	RCC_PLL_init();                         // System Clock = 84MHz	--> 16MHz to calculate 84MHz in PLL_init
		//	RCC_HSI_init();										// HSI = 16MHz 
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
}
