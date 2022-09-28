/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-09-26 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  Tutorial Digital In
*					 Toggle LED
* 
******************************************************************************
*/


// GPIO Mode			 : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			 : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)


#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"
#include "ecSysTick.h"

#define LED_PIN 	5
#define BUTTON_PIN 13

void setup(void);

	
int main(void) { 
	// Initialiization
	setup();
	
	// Inifinite Loop 
	while(1){
		
		if(GPIO_read(GPIOC, BUTTON_PIN) == 0)		{
			
			bitToggle(GPIOA, LED_PIN);
		}
		
		delay_ms(100);
		
	}
}


// Initialiization 
void setup(void)
{
	
	SysTick_init();
	
	RCC_HSI_init();	
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()
	
	GPIO_otype(GPIOA, LED_PIN, OPDRAIN);	// LED open drain
	
	GPIO_pupd(GPIOA, LED_PIN, PULLUP);		// LED PULLUP 
	GPIO_ospeed(GPIOA, LED_PIN, MSPEED);	// LED Medium speed
	GPIO_pupd(GPIOC, BUTTON_PIN, PULLUP); // button pull up 

	GPIO_write(GPIOA, LED_PIN, LOW); 			// claer LED

		
}


// using XOR for make toggle 
// 
