/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-09-22 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  Tutorial Digital In
*					 - Turn on LED LD2 while Button B1 is pressing
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
#define BUTTON_PIN 13

int main(void) {	
	/* Part 1. RCC GPIOA Register Setting */
		RCC_GPIOA_enable();
		RCC_GPIOC_enable();
	

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

	
	/* Part 3. GPIO Register Setting for INPUT*/			
		// GPIO Mode Register
		GPIOC->MODER &= 	~(3UL << (BUTTON_PIN * 2));		// 00: claer
		GPIOC->MODER |= 	0 << (BUTTON_PIN * 2);				// 00: Input	 		
   
		// GPIO Pull-Up/Pull-Down Register 
		GPIOC->PUPDR 	&=  ~(3UL<<( BUTTON_PIN *2)); // clear
		GPIOC->PUPDR  |=	2UL <<( BUTTON_PIN *2);				// 10: Pull-down		    
	 
	 
	/* Part 4. Deal loop  */	
		while(1){
			unsigned int btVal=0;
			//Read bit value of Button
			btVal=(GPIOC->IDR) & (1UL << BUTTON_PIN);	
			
			if(btVal == 0)
				
				GPIOA->ODR |= (1UL << LED_PIN);	 	
			
			else
				
				GPIOA->ODR &= ~(1UL << LED_PIN); 
			
		}
}
