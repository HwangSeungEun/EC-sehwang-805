/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Hanse Cho LAB
Created          : 10-20-2022
Modified         : 10-24-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for EC_Final_Project
/----------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecSysTick.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecRGBCS.h"

#define END_CHAR 13
#define MAX_BUF 100

#define DELIV1_LASER_PORT GPIOB
#define DELIV2_LASER_PORT GPIOC
#define DELIV3_LASER_PORT GPIOC
#define DELIV1_LASER_PIN 0
#define DELIV2_LASER_PIN 1
#define DELIV3_LASER_PIN 0

#define S0_PORT 		GPIOC
#define S0_PIN 			9
#define S1_PORT 		GPIOB
#define S1_PIN  		8
#define S2_PORT 		GPIOC
#define S2_PIN  		8
#define S3_PORT 		GPIOC
#define S3_PIN  		6
#define CS_EN_PORT 	GPIOC
#define CS_EN_PIN		5
#define RGB1_PORT 	GPIOA
#define RGB1_PIN 		0
#define RGB2_PORT 	GPIOA
#define RGB2_PIN 		6
#define RGB3_PORT 	GPIOB
#define RGB3_PIN 		6

uint8_t pcData = 0;
uint8_t buffer[MAX_BUF] = {0, };
int bReceive = 0;
int idx = 0;

int cnt = 0;
float rgb1_val1 = 0;

int Product = 0;
int Delivery = 0;

uint32_t rgb1_ovf_cnt = 0;
uint8_t rgb1_rise = 0;
uint8_t rgb1_fall = 0;
uint8_t rgb1_val = 0;

uint32_t rgb2_ovf_cnt = 0;
uint8_t rgb2_rise = 0;
uint8_t rgb2_fall = 0;
uint8_t rgb2_val = 0;

uint32_t rgb3_ovf_cnt = 0;
uint8_t rgb3_rise = 0;
uint8_t rgb3_fall = 0;
uint8_t rgb3_val = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Give me RGB:");
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		if(pcData == 'R' || pcData == 'G' || pcData == 'B'){Product = pcData;}
		else if(pcData == '1' || pcData == '2' || pcData == '3'){Delivery = pcData;}
		if(Product != 0 && Delivery != 0){
			if(Product == 'R'){RGBCS_config(RGB_R);}
			else if(Product == 'G'){RGBCS_config(RGB_G);}
			else if(Product == 'B'){RGBCS_config(RGB_B);}
			printf("Product: ");
			printf("%c\r\n" ,Product);
			rgb1_val1 += rgb1_val;
			printf("%d\r\n" ,rgb1_val);
			printf("%f\r\n" ,rgb1_val1);
			if(cnt >= 50){break;}
			cnt++;
			if(rgb1_val < 17){GPIO_write(GPIOA,5,1);}
		}
		delay_ms(100);
	}
	rgb1_val1 = rgb1_val1/cnt;
	printf("%f",rgb1_val1);
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	USART_init(USART2, 9600);
	
	// Delivery laser pin
	GPIO_init_out(DELIV1_LASER_PORT,DELIV1_LASER_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(DELIV2_LASER_PORT,DELIV2_LASER_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(DELIV3_LASER_PORT,DELIV3_LASER_PIN, OUTPUT,1,0,EC_PU);
	
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	RGBCS_init();
	RGBCS_freqScale(RGB_2_PER);
	
	
	GPIO_init_out(GPIOA,8, OUTPUT,1,0,EC_PU);
	GPIO_write(GPIOA,8,1);
	
	GPIO_init_out(GPIOA,5, OUTPUT,1,0,EC_PU);
	GPIO_init(GPIOC, 13, INPUT);
	GPIO_pupdr(GPIOC, 13, EC_PU);

	// Input Capture configuration -----------------------------------------------------------------------	
	RGBCS_IC_config();

}

void USART2_IRQHandler(){         //USART2: COM-FT(Factory)(MCU1)
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		printf("%c", pcData);
		if (pcData == END_CHAR) printf("\r\n");
		printf("\r\nGive me Spot:");
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		rgb1_ovf_cnt++;													// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 1)){ 									// TIM2_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb1_rise = TIM2->CCR1;									// Capture TimeStart
		clear_CCIF(TIM2, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2, 2)){ 								// TIM2_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb1_fall = TIM2->CCR2;										// Capture TimeEnd
		rgb1_val = ((rgb1_fall-rgb1_rise) + (0xFFFF+1)*rgb1_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		rgb1_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 2);								  // clear capture/compare interrupt flag 
	}
}

void TIM3_IRQHandler(void){
	if(is_UIF(TIM3)){                     	// Update interrupt
		rgb2_ovf_cnt++;													// overflow count
		clear_UIF(TIM3);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM3, 1)){ 									// TIM3_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb2_rise = TIM3->CCR1;									// Capture TimeStart
		clear_CCIF(TIM3, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM3, 2)){ 								// TIM3_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb2_fall = TIM3->CCR2;										// Capture TimeEnd
		rgb2_val = ((rgb2_fall-rgb2_rise) + (0xFFFF+1)*rgb2_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		rgb2_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 2);								  // clear capture/compare interrupt flag 
	}
}

void TIM4_IRQHandler(void){
	if(is_UIF(TIM4)){                     	// Update interrupt
		rgb3_ovf_cnt++;													// overflow count
		clear_UIF(TIM4);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM4, 1)){ 									// TIM4_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb3_rise = TIM4->CCR1;									// Capture TimeStart
		clear_CCIF(TIM4, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM4, 2)){ 								// TIM4_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb3_fall = TIM4->CCR2;										// Capture TimeEnd
		rgb3_val = ((rgb3_fall-rgb3_rise) + (0xFFFF+1)*rgb3_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		rgb3_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM4, 2);								  // clear capture/compare interrupt flag 
	}
}