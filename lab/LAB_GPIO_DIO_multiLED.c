/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-09-27 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  Tutorial Digital In
*					 Toggle Multi LED
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

#define PA5 		5
#define PA6 		6
#define PA7 		7
#define PB6 		6

#define BUTTON_PIN 13

void setup(void);

int main(void) { 
	// Initialiization
	setup();

	// Inifinite Loop
	 
	int count_button = 0;
	int flag = 0;
	
	while(1){
		
		if(GPIO_read(GPIOC, BUTTON_PIN) == 0)		{
			
			count_button ++; 
			flag = count_button % 4;
					
			switch(flag){
			
				case 1:
					 
					bitToggle(GPIOA, PA5); 		// led 1 on : stm led
					if(count_button > 4){
						bitToggle(GPIOB, PB6);  // if after sencond terms led 4 off
					}
					break;

				case 2:
					
					bitToggle(GPIOA, PA5);		// led 1 off : red led
					bitToggle(GPIOA, PA6);		// led 2 on
					break;				
					
				case 3:
					
					bitToggle(GPIOA, PA6);		// led 2 off : green led
					bitToggle(GPIOA, PA7);    // led 3 on
					break;

				case 0:
					
					bitToggle(GPIOA, PA7);		// led 3 off : white led
					bitToggle(GPIOB, PB6);    // led 4 on
					break;	
			}
		}
		
		delay_ms(30);										// software debouncing
		
	}
}



// Initialiization 
void setup(void)
{
	
	RCC_HSI_init();	
	SysTick_init();
	
	// button setup
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_pupd(GPIOC, BUTTON_PIN, PULLUP); // button pull up 
	
	// led 1 setup
	GPIO_init		(GPIOA, PA5, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype		(GPIOA, PA5, PUSHPULL);	// LED open drain
	GPIO_pupd		(GPIOA, PA5, PULLUP);		// LED PULLUP 
	GPIO_ospeed		(GPIOA, PA5, MSPEED);		// LED Medium speed
	GPIO_write		(GPIOA, PA5, LOW); 		// claer LED
	
	// led 2 setup
	GPIO_init		(GPIOA, PA6, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype		(GPIOA, PA6, PUSHPULL);	// LED open drain
	GPIO_pupd		(GPIOA, PA6, PULLUP);		// LED PULLUP 
	GPIO_ospeed		(GPIOA, PA6, MSPEED);		// LED Medium speed
	GPIO_write		(GPIOA, PA6, LOW); 		// claer LED	
										
	// led 3 setup
	GPIO_init		(GPIOA, PA7, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype		(GPIOA, PA7, PUSHPULL);	// LED open drain
	GPIO_pupd		(GPIOA, PA7, PULLUP);		// LED PULLUP 
	GPIO_ospeed		(GPIOA, PA7, MSPEED);		// LED Medium speed
	GPIO_write		(GPIOA, PA7, LOW); 		// claer LED	
										
	// led 4 setup
	GPIO_init		(GPIOB, PB6, OUTPUT);		// calls RCC_GPIOA_enable()
	GPIO_otype		(GPIOB, PB6, PUSHPULL);	// LED open drain
	GPIO_pupd		(GPIOB, PB6, PULLUP);		// LED PULLUP 
	GPIO_ospeed		(GPIOB, PB6, MSPEED);		// LED Medium speed
	GPIO_write		(GPIOB, PB6, LOW); 		// claer LED
	
}

