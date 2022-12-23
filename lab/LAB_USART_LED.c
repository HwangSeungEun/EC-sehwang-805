/**
******************************************************************************
* @author  Hwang SeungEun
* @Mod		 2022-11-14 by Hwang SeungEun
* @brief   Embedded Controller:  USART communication
*
******************************************************************************
*/
#include "ecInclude.h"

// USART2 : MCU to PC via usb 
// USART1 : MCU to MCU2

#define END_CHAR 	13
#define MAX_BUF 	10

// char data
uint8_t mcu2Data = 0;
uint8_t pcData = 0;

uint8_t buffer[MAX_BUF]={0, };
int idx = 0;

int bReceive =0;
int ledOn = 0;


void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	USART_write(USART1,(uint8_t*) "Hello bluetooth\r\n",17);
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		if (bReceive==1 && buffer[0]=='L'){
			printf("buffer: %s\r\n", buffer);
			if (buffer[0] == 76 ){    										// 76 == L
				if 			(buffer[1] == '0') ledOn = 0;				// L1 == off	
				else if (buffer[1] == '1') ledOn = 1;				// L0 == on	
				else 		 printf("ERROR : Wrong command\r\n");
			}
			else
				printf("ERROR : Wrong command\r\n");
			
			bReceive = 0;																	// flag reset
			memset(buffer, 0, sizeof(char) * MAX_BUF);
		}
				
		GPIO_write(GPIOA, 5, ledOn);
		delay_ms(500);
	}
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_ms_init(1);
	
	// USART congfiguration
	USART_init(USART2, 38400);
	USART_begin(USART1,  GPIOA, 9, GPIOA, 10, 9600); 	// PA10 - RXD , PA9 - TXD

	// GPIO configuration
	GPIO_out_set(GPIOA, PA5, PULLUP, MSPEED, PUSHPULL); // PA5 LED pin
}

void USART1_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART1)){
		mcu2Data = USART_getc(USART1);						
		if(mcu2Data == END_CHAR) {
			// 여기서 키고 인식한 다음 버퍼에서 처음부터 
			bReceive = 1;  
			idx = 0;
		}
		else{
			if(idx > MAX_BUF){
				idx = 0;
				memset(buffer, 0, sizeof(char) * MAX_BUF);
				printf("ERROR : Too long string\r\n");
			}
			buffer[idx] = mcu2Data;
			idx++;
		}
	}
}

void USART2_IRQHandler(){		//USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART1, &pcData, 1);	// transmit char to USART1
		
		printf("%c", pcData); 						// echo to sender(pc)
		
		if(pcData == END_CHAR){
			printf("\r\n"); 							// to change line on PC display
		}
	}
}
