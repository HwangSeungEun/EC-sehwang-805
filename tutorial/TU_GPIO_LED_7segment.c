/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-09-22 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  Tutorial Digital In
*					 - Turn on LED LD2 while Button B1 is pressing
* 
******************************************************************************
*/





#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();

	// Inifinite Loop ----------------------------------------------------------
	while (1) {
		GPIO_write(GPIOA, 5, LOW);
		GPIO_write(GPIOA, 6, LOW);
		GPIO_write(GPIOA, 7, LOW);
		GPIO_write(GPIOB, 6, LOW);
		GPIO_write(GPIOC, 7, LOW);
		GPIO_write(GPIOA, 9, LOW);
		GPIO_write(GPIOA, 8, LOW);
		GPIO_write(GPIOB, 10, LOW);
	}
}

void setup(void) {
	RCC_HSI_init();
	GPIO_init(GPIOA, 5, OUTPUT);
	GPIO_init(GPIOA, 6, OUTPUT);
	GPIO_init(GPIOA, 7, OUTPUT);
	GPIO_init(GPIOB, 6, OUTPUT);
	GPIO_init(GPIOC, 7, OUTPUT);
	GPIO_init(GPIOA, 9, OUTPUT);
	GPIO_init(GPIOA, 8, OUTPUT);
	GPIO_init(GPIOB, 10, OUTPUT);
}