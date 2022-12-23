#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecSysTick.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecRGBCS.h"
#include "ecMCU1.h"

#define WAIT_DELV_FLAG 0
#define WAIT_PRDCT_FLAG 1
#define START_FLAG 2
uint8_t flag = 0;

#define END_CHAR 13
#define MAX_BUF 100

#define WAIT_FINISH_DELV_STATE 4

uint32_t zgbData = 0;
#define Rst_zgbData 0
uint32_t pcData = 0;

int Product = 0;
int Delivery = 0;
#define Rst_Delv 0
#define Rst_Prdct 0

uint32_t rgb1_ovf_cnt = 0;
uint32_t rgb1_rise = 0;
uint32_t rgb1_fall = 0;
uint32_t rgb1_val = 0;

#define LED_PIN 	5

PWM_t PWM1; // product = R
PWM_t PWM2; // product = G
PWM_t PWM3; // product = B

PWM_t PWM4;

void setup(void);
float servodeg2pwmratio(float servodeg);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	uint32_t state = 0;	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		if		 (Delivery == Rst_Delv){flag = WAIT_DELV_FLAG;}
		else if(Product == Rst_Prdct){flag = WAIT_PRDCT_FLAG;}
		else												 {flag = START_FLAG;}
		
		switch(flag){
			case WAIT_DELV_FLAG:
				// wait for Delivey input
				printf("\033[H\033[2J"); 
				printf("Input_Delivery Adress(1,2,3): "); 
				delay_ms(2000); 
				continue;
			
			case WAIT_PRDCT_FLAG:
				// wait for Product input
				printf("\33[2K\r");
				printf("Input_Product(R,G,B): "); 
				delay_ms(2000); 
				continue;
			
			case START_FLAG:
				// Receive the delivery address and Product
				if(Product == Rst_Prdct){state = 0;}
				else if(Product == 'R') {state = 1; RGBCS_config(RGB_R); }
				else if(Product == 'G') {state = 2; RGBCS_config(RGB_G); }
				else if(Product == 'B') {state = 3; RGBCS_config(RGB_B); }
				MCU1_Out(state);
				MCU1_Out(WAIT_FINISH_DELV_STATE);
				break;
		}
		
		// wait for finish delivery
		while(!(zgbData == 'S' || zgbData == 'F')){}
			
		// start when finish delivery
		if		 (zgbData == 'S'){printf("Success: ");}
		else if(zgbData == 'F'){printf("Fail: ");}
		printf("Delivery Address = %c, Product = %c\r\n",Delivery, Product);
		PWM_duty(&PWM4,servodeg2pwmratio(89));
		
		Delivery = Rst_Delv;
		Product = Rst_Prdct;
		zgbData = Rst_zgbData;
		delay_ms(10000);
	}
}
// Initialiization  
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_init(1);
	USART_init(USART2, 9600);
	USART_begin(USART6, GPIOA, 11, GPIOA, 12, 9600);
	
	MCU1_init(&PWM1,GPIOB, 6,&PWM2, GPIOB, 7,&PWM3, GPIOB, 9);	
	
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	RGBCS_init();
	RGBCS_freqScale(RGB_20_PER);
	GPIO_init_out(GPIOC,10, OUTPUT,1,0,EC_PU);
	GPIO_write(GPIOC,10,1);
	
	GPIO_init_out(GPIOA,5, OUTPUT,1,0,EC_PU);
	PWM_init_ms(&PWM4, GPIOA, 6,20);
	PWM_duty(&PWM4,servodeg2pwmratio(90));
	
	IC_t RGB1;												// Input Capture for echo
	ICAP_init(&RGB1, RGB1_PORT, RGB1_PIN);    		// PB10 as input caputre
 	ICAP_counter_us(&RGB1, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&RGB1, 1, IC_RISE);   	// TIM2_CH1 as IC1 , rising edge detect
	ICAP_setup(&RGB1, 2, IC_FALL);   	// TIM2_CH1 as IC2 , falling edge detect
	
}

void USART2_IRQHandler(){         //USART2: COM-FT(Factory)(MCU1)
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART6, &pcData, 1);
		printf("%c", pcData);
		if(pcData == '1' || pcData == '2' || pcData == '3'){Delivery = pcData; printf("\033[H\033[2J"); printf("Delivery: %c\r\n", pcData);}
		else if(pcData == 'R' || pcData == 'G' || pcData == 'B'){Product = pcData;}
		if (pcData == END_CHAR) printf("\r\n");
	}
}

void USART6_IRQHandler(){
	if(is_USART_RXNE(USART6)){
		zgbData = USART_getc(USART6);
//		printf("%c", zgbData);	
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
		
		if(Product == 'R'){if(rgb1_val < 11){PWM_duty(&PWM4,servodeg2pwmratio(0));}}
		else if(Product == 'G'){if(rgb1_val < 11){PWM_duty(&PWM4,servodeg2pwmratio(0));}}
		else if(Product == 'B'){if(rgb1_val < 11){PWM_duty(&PWM4,servodeg2pwmratio(0));}}
		
		rgb1_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 2);								  // clear capture/compare interrupt flag 
	}
}

float servodeg2pwmratio(float servodeg){ // unit conversion deg to ratio of pwm
	return (0.1/180.0)*servodeg + 0.025; // 0.5ms~2.5ms => 0.025 ~ 0.125 ratio
}