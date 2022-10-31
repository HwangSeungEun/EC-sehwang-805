/**
  ******************************************************************************
  * @author  SSSLAB
	* @Mod		 2021-8-12 by YKKIM  	
  * @brief   Embedded Controller:  Tutorial Digital Out
  *					 - Turn on LED LD2
  * 
  ******************************************************************************
*/


// GPIO Mode			 : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			 : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)


#include "stm32f4xx.h"
#include "ecRCC.h"

#define LED_PIN    5		//LD2


int main(void) {	
		/* Part 1. RCC GPIOA Register Setting */
		RCC_GPIOA_enable();
		
		/* Part 2. GPIO Register Setting */			
		// GPIO Mode Register
		GPIOA->MODER &= ~(3UL << (LED_PIN * 2));// Clear '00' for Pin 5
		GPIOA->MODER |= 1UL << (LED_PIN * 2);		// Set '01' for Pin 5
		
		// GPIO Output Type Register  
		GPIOA->OTYPER &= 	~(1UL << LED_PIN);		// 0:Push-Pull   
		
		// GPIO Pull-Up/Pull-Down Register 
		GPIOA->PUPDR  &= 	~(3UL << (LED_PIN * 2));		// 00: claer
		GPIOA->PUPDR  |= 	(0 << (LED_PIN * 2));			// 00: write
		
		// GPIO Output Speed Register 
		GPIOA->OSPEEDR &= ~(3UL << (LED_PIN * 2));
		GPIOA->OSPEEDR |= 2UL << (LED_PIN * 2);	//10:Fast Speed
	
		// Dead loop & program hangs here
		while(1){
			//	 GPIOA->ODR = 1UL << LED_PIN; 	// Set LED_PIN = H, others=L
			GPIOA->ODR |= (1UL << LED_PIN);	 		// Change only LED_PIN = H  
		}
}