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
#include "ecMCU1.h"

#define END_CHAR 13
#define MAX_BUF 100

#define DELIV1_LASER_PORT GPIOB
#define DELIV2_LASER_PORT GPIOC
#define DELIV3_LASER_PORT GPIOC
#define DELIV1_LASER_PIN 0
#define DELIV2_LASER_PIN 1
#define DELIV3_LASER_PIN 0

PWM_t PWM5;

uint32_t zgbData = 0;
uint32_t pcData = 0;
uint32_t buffer[MAX_BUF] = {0, };
int bReceive = 0;
int idx = 0;

uint8_t Product = 0;
uint8_t Delivery = 0;
uint8_t Feedback = 0;

uint32_t rgb1_ovf_cnt = 0;
uint32_t rgb1_rise = 0;
uint32_t rgb1_fall = 0;
uint32_t rgb1_val = 0;

uint32_t rgb2_ovf_cnt = 0;
uint32_t rgb2_rise = 0;
uint32_t rgb2_fall = 0;
uint32_t rgb2_val = 0;

uint32_t rgb3_ovf_cnt = 0;
uint32_t rgb3_rise = 0;
uint32_t rgb3_fall = 0;
uint32_t rgb3_val = 0;

uint32_t rgb_val = 0;
uint32_t rgb_val_min = 0;

uint32_t Red_val = 0;
uint32_t Green_val = 0;
uint32_t Blue_val = 0;
uint32_t RGB_cnt = 0;
uint32_t RGB_cnt_min = 0;
uint32_t Detect_val = 0;

uint32_t Detect_cnt = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		if(Product != 0 && Delivery != 0){
			// setting rgb sensor config
			RGBCS_config(RGB_cnt);
			if		 (Delivery == '1'){rgb_val = rgb1_val; GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,1); GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,0); GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,0);}
			else if(Delivery == '2'){rgb_val = rgb2_val; GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,0); GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,1); GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,0);}
			else if(Delivery == '3'){rgb_val = rgb3_val; GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,0); GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,0); GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,1);}
			if(300<rgb_val){
				//// Q. why RGB_cnt = 1 -> rgb vale = R? 
				//// A. we get rgb_val before start this loop, and when start loop we set RGBCS_config, this setting is applied next loop.
				//// RGBCS_config(RGB_cnt);
				delay_ms(1000);
				
			// rgb offset set					
			RGB_OFFSET_SETTING(Delivery, RGB_cnt, &rgb_val);
				
				// display cnt, rgb value 
				printf("cnt:%d, %d\r\n",RGB_cnt,rgb_val);
				
				// select min value of cnt, rgb value of R,G,B
				if(rgb_val > rgb_val_min){rgb_val_min = rgb_val_min; RGB_cnt_min = RGB_cnt_min;}
				else if(rgb_val <= rgb_val_min){rgb_val_min = rgb_val; RGB_cnt_min = RGB_cnt;}
				
				// 
				if(RGB_cnt == 2 && rgb_val_min != 0){
					if(Detect_cnt >= 1){
						printf("rgb_val_min: %d, %d\r\n",rgb_val_min,RGB_cnt_min);
						if(RGB_cnt_min == 0){Detect_val = 'B';}
						else if(RGB_cnt_min == 1){Detect_val = 'R';}
						else if(RGB_cnt_min == 2){Detect_val = 'G';}
						
						if(Product == Detect_val){Feedback = 'S'; USART_write(USART6, &Feedback, 1);}
						else 										 {Feedback = 'F'; USART_write(USART6, &Feedback, 1);}
					//	printf("Finish\r\n, Feedback: %c",Feedback);
						
						Product = 0;
						Delivery = 0;
						Feedback = 0;
						GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,0);
						GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,0);
						GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,0);
						
					}
					Detect_cnt++;
				}
				
				// reset detect cnt
				if(Detect_cnt >= 5 ){Detect_cnt = 0;}
				RGB_cnt++;
				
				// when get R,G,B value all, reset all
				if(RGB_cnt>2){RGB_cnt = 0; rgb_val_min = 10e6; RGB_cnt_min = 10;}
			}
			
			// when no car but lane detect, print no detect(product)
			else{
				printf("No detect\r\n"); 
				delay_ms(1000);
			}
			
		}
		
	}
	
	
			
			
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	USART_init(USART2, 9600);
	USART_begin(USART6, GPIOA, 11, GPIOA, 12, 9600);
	
	// Delivery laser pin
	GPIO_init_out(DELIV1_LASER_PORT,DELIV1_LASER_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(DELIV2_LASER_PORT,DELIV2_LASER_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(DELIV3_LASER_PORT,DELIV3_LASER_PIN, OUTPUT,1,0,EC_PU);
	
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	RGBCS_init();
	RGBCS_freqScale(RGB_2_PER);
	GPIO_init_out(GPIOC,10, OUTPUT,1,0,EC_PU);
	GPIO_write(GPIOC,10,1);
	PWM_init_us(&PWM5, GPIOA, 8,1000);
	PWM_duty(&PWM5,0.6);
	
	GPIO_init_out(GPIOA,5, OUTPUT,1,0,EC_PU);
	GPIO_init(GPIOC, 13, INPUT);
	GPIO_pupdr(GPIOC, 13, EC_PU);

	// Input Capture configuration -----------------------------------------------------------------------	
	RGBCS_IC_config();
	

}

void USART6_IRQHandler(){
	if(is_USART_RXNE(USART6)){
		zgbData = USART_getc(USART6);
		if		 (zgbData == 'R' || zgbData == 'G' || zgbData == 'B'){Product = zgbData;}
		else if(zgbData == '1' || zgbData == '2' || zgbData == '3'){Delivery = zgbData;}
		printf("%c", zgbData);	
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