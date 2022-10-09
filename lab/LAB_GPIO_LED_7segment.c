/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-09-29 by Seung-Eun Hwang  	
* @brief   Embedded Controller:  - LED 7 segment
*					 
* 
******************************************************************************
*/

//#include "stm32f4xx.h"
//#include "ecRCC.h"
//#include "ecGPIO.h"
//#include "ecSysTick.h"
//#include "ecFunc.h"

#include "ecInclude.h"



void setup(void);

int main(void) {
	// Initialization --------------------------------------------------------
	setup();

	// matrix initialozation -------------------------------------------------
	
	int cnt = 0;
	int flag = 0;


	
	// Inifinite Loop ----------------------------------------------------------
	while (1) {

		if (GPIO_read(GPIOC, BUTTON_PIN) == 0) {

			cnt++;
			flag = cnt % 10;
			
			sevensegment_decoder(flag);

		}
			
		delay_ms(50);										// software debouncing

	}
}

void setup(void) {

	RCC_HSI_init();
	SysTick_init();

	// button setup
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_pupd(GPIOC, BUTTON_PIN, NOPUPD); // button pull up 
	
	// seven segment setup
	sevensegment_init();
	
	
}