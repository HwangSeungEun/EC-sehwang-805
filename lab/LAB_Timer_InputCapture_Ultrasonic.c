/**
******************************************************************************
* @author  SeungEun Hwang
* @Mod		 2022-11-03 by SeungEun Hwang 	
* @brief   Embedded Controller:  LAB_Timer_InputCapture
*					 - _________________________________
* 
******************************************************************************
*/

#include "ecInclude.h"

#define PA6			6

uint32_t ovf_cnt = 0;
float distance = 0;
float timeInterval = 0;
float timeSt = 0;
float timeEnd = 0;


float PulseWidth = 10;

void setup(void);

int main(void){
	
	setup();
	
	// Setting Time = about 6~7 sec.
	printf("Getting ready to run. Please wait a moment.                         		\n"); delay_ms(2222);
	printf("Getting ready to run. Please wait a moment.                         		\n"); delay_ms(2222);
	printf("Getting ready to run. Please wait a moment.                         		\n"); delay_ms(2222);

	
	while(1){
//		distance = (float) timeInterval * 340.0 / 2.0 / 10.0; 	// [mm] -> [cm]
		
		if(distance > 2 && distance < 400 ){
			printf("\r%f[cm]                         	", distance);
//			delay_ms(10);
		}else{
			printf("\r garbage value : no recognition ");
//			delay_ms(10);
		}

		delay_ms(50);
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		ovf_cnt++;													// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 3)){ 									// TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
		timeSt = TIM2->CCR3;	// Capture TimeStart
		clear_CCIF(TIM2, 3);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2, 4)){ 								// TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
		timeEnd = TIM2->CCR4;										// Capture TimeEnd
		clear_CCIF(TIM2, 4);								  // clear capture/compare interrupt flag 
		
		timeInterval = PulseWidth*((timeEnd - timeSt)+(ovf_cnt * 0xFFFF)); 					// Total time of echo pulse (10us * counter pulses -> [msec] unit)
		distance = (float) timeInterval / 58; 	// [mm] -> [cm]
		ovf_cnt = 0;                        	// overflow reset	
	}
}

void setup(){

	RCC_PLL_init(); 
	
	SysTick_ms_init(1);
	UART2_init();
  
// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;												// PWM1 for trig
	PWM_init(&trig, GPIOA, PA6, NOPUPD, FSPEED, PUSHPULL, UPPCOUNT, 1);			 						// PA_6: Ultrasonic trig pulse
	PWM_period_us(&trig, 50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig, 10);   	// PWM pulse width of 10us
	
// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;												// Input Capture for echo
	ICAP_init(&echo, GPIOB, PB10);   		// PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, IC_RISE);   	// TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, IC_FALL);   	// TIM2_CH3 as IC4 , falling edge detect
	
	NVIC_SetPriority(TIM2_IRQn, 2);						    	// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);									   		// TIM2 interrupt request enable
}
