/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  LAB ___
*					 - _________________________________
* 
******************************************************************************
*/

#include "ecInclude.h"

#define END_CHAR 13
#define A 0
#define B 1
#define MAX_BU
#define BUTTON_PIN	13

//IR parameter//
uint32_t IR1, IR2, ILLUM;
int flag = 0;
int bluetooth = 0;

int seqCHn[16] = {8,9, 1, };  // 16비트 array 세팅
uint8_t pcData = 0;
uint8_t mcu2Data = 0;
uint8_t btData = 0;
int 		idx = 0;

// rc카 속도 및 모터 회전 방향 설정
float 	speed_A = 0.0;
float 	speed_B = 0.0;

int			dir_A 	= HIGH;
int 		dir_B 	= HIGH;

// ultrasonic
#define PA6			6

uint32_t ovf_cnt 		= 0;
float distance			= 0.0;
float timeInterval 	= 0.0;
float timeSt 				= 0.0;
float timeEnd 			= 0.0;

float PulseWidth 		= 10;
uint8_t data 				= 0;

void setup(void);
void EXTI15_10_IRQHandler(void);
void ADC_IRQHandler(void);

typedef struct {
	
		GPIO_TypeDef *port;
		int pin; // TIM3 Ch3
	
}_Pin;


_Pin dcPwmPin[2] = {
	{GPIOA, 8}, // TIM1 ch1
	{GPIOA, 9}	// TIM1 ch2
};

PWM_t dcPwm[2];

_Pin dcDirPin[2] = {
	{GPIOC, 9}, 	//모터 A
	{GPIOC, 8}		//모터 B
};

	
int main(void) { 

	// Initialiization --------------------------------------------------------
	setup();
	// Setting Time = about 6~7 sec.
//	printf("Getting ready to run. Please wait a moment.                         		\n"); delay_ms(2222);

	PWM_duty(&dcPwm[A], 1);
	PWM_duty(&dcPwm[B], 1);
	GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
	GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진
	delay_ms(300);  // 3초 기다려보자 
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		printf("IR1 = %d \r\n",IR1);
		printf("IR2 = %d \r\n",IR2);

		printf("ILLUM =        %d \r\n",ILLUM);
		printf("\r\n");
//		if (IR1 > 1000 && IR2 < 1000) {
//			printf("GO RIGHT\r\n");
//		}
//		else if (IR2 > 1000 && IR1 < 1000) {
//			printf("GO LEFT\r\n");
//		}
//		else {
//			printf("GO FRONT\r\n");
//		}	
//		
//			printf("bluetooth = %d \r\n", bluetooth);
//		if((distance > 2 && distance < 7)){
//				PWM_duty(&dcPwm[A], 1);
//				PWM_duty(&dcPwm[B], 1);
//				GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
//				GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진
//		}	else {
//	
//			PWM_duty(&dcPwm[A], 1 - speed_A);
//			PWM_duty(&dcPwm[B], 1 - speed_B );
//			GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
//			GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진

//		}

			PWM_duty(&dcPwm[A], 1 - speed_A);
			PWM_duty(&dcPwm[B], 1 - speed_B );
			GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
			GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진


		delay_ms(50);
//
	}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_ms_init(1);
	
	// Button intterupt
	GPIO_in_set(GPIOC, BUTTON_PIN, PULLUP);
	EXTI_init(GPIOC, BUTTON_PIN,FALL,0);
	
	
	// USART setting
	USART_init(USART2, 9600);
	USART_begin(USART1, GPIOA, 9, GPIOA, 10, 9600); 	// PA9: TXD , PA10: RXD
	
	// PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin, int pupd, int speed, int type ,int dir, uint32_t msec);
	PWM_init(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin, FSPEED, PULLDO, PUSHPULL, UPPCOUNT,1);
	PWM_init(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin, FSPEED, PULLDO, PUSHPULL, UPPCOUNT,1);
	
	PWM_period_us(&dcPwm[A], 100);
	PWM_period_us(&dcPwm[B], 100);
	
	// 출력이 HIGH일 때 는 숫자가 작을 수록 빨라진다 그래서 출력을 입력해주는 숫자는 내가 보기 쉽게 0~1까지로 설정하고 
	// duty 입력을 해줄 때 1 - speed_duty 로 계산해준다
	PWM_duty(&dcPwm[A], 0.0);
	PWM_duty(&dcPwm[B], 0.0);
	
	for (int i = 0; i < 2; i++){
		GPIO_out_set(dcDirPin[i].port, dcDirPin[i].pin, PULLDO,HSPEED,PUSHPULL);
	}
	
	// A는 HIGH일 때 전진
	GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, HIGH);
	// B는 HIGH일 때 전진
	GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, HIGH);
	
	// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;												// PWM1 for trig
	PWM_init(&trig, GPIOA, PA6, NOPUPD, FSPEED, PUSHPULL, UPPCOUNT, 1);			 						// PA_6: Ultrasonic trig pulse TIM3 ch1
	
	PWM_period_us(&trig, 50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig, 10);   	// PWM pulse width of 10us
	
	// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;												// Input Capture for echo
	ICAP_init(&echo, GPIOB, PB10);   		// PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, IC_RISE);   	// TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, IC_FALL);   	// TIM2_CH3 as IC4 , falling edge detect	
	
	// ADC setting
//  ADC_init(GPIOB, 0, TRGO);   //  
//	ADC_init(GPIOB, 1, TRGO);		//
////	ADC_init(GPIOA, 1, TRGO); // SS_ILLUM

//	// ADC channel sequence setting
//	ADC_sequence(2, seqCHn);	// 채널 2개 
//	
//	// ADON, SW Trigger enable
//	ADC_start();

	ADC_INJ_init(GPIOB, 6, TRGO);
	ADC_INJ_init(GPIOB, 7, TRGO);
	
	ADC_INJ_sequence(2, seqCHn);	// 채널 2개 
	
	
//	ADC_init(GPIOA, 1, TRGO); // SS_ILLUM
	
	NVIC_SetPriority(ADC_IRQn, 2);						    	
	NVIC_EnableIRQ(ADC_IRQn);									  
	
	NVIC_SetPriority(TIM2_IRQn, 1);						    	// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);									   		// TIM2 interrupt request enable
}


void ADC_IRQHandler(void){
	if((is_ADC_OVR())){
		clear_ADC_OVR();
	}
	
//	if(is_ADC_EOC()){       //after finishing sequence
//			if (flag==0){
//				IR1 = ADC_read();
//			}  
//			else if (flag==1){
//				IR2 = ADC_read();
//			}
//			else if(flag == 2){
//				ILLUM =  ADC_read();
//			}
//		flag += 1;
//		if(flag > 2){
//			flag = 0;
//		}
//	}

	
	if(bluetooth == 0){
		if (IR1 > 2000 && IR2 < 2000) { 
			// 직진 우회전
			data = 'I';
			speed_A = 0.9;
			speed_B = 0.6;
			dir_A		= HIGH;
			dir_B		= HIGH;
		}			
		else if (IR2 > 2000 && IR1 < 2000) {
			// 직진 좌회전
			data = 'G';
			speed_A = 0.6;
			speed_B = 0.96 ;
			dir_A		= HIGH;
			dir_B		= HIGH;
		}else {
			data = 'F';
			speed_A = 0.7;
			speed_B = 0.7;  
			dir_A		= HIGH;
			dir_B		= HIGH;
		}
		
		if((distance > 2 && distance < 8)){
			speed_A = 0.1;
			speed_B = 0.1;
			dir_A		= HIGH;
			dir_B		= HIGH;
		}
	}
}

void USART1_IRQHandler(){         //USART1 INT 
	if(is_USART_RXNE(USART1)){
		btData = USART_getc(USART1);
		
		data = btData;
		if(data == 'W'){
			bluetooth ^= 1;
		}
		
		if (bluetooth == 1){
			switch(data){
			// 전진	
			case 'F':  // 전진을 입력하면 조금씩 좌회전 한다.
					speed_A = 0.8;
					speed_B = 0.82;  
					dir_A		= HIGH;
					dir_B		= HIGH;
					break;
			// 후진
			case 'B':
					speed_A = 0.4;
					speed_B = 0.4;
					dir_A		= LOW;
					dir_B		= LOW;
					break;	
			// 우회전
			case 'R':
					speed_A = 0.4;
					speed_B = 0.6;
					dir_A		= LOW;
					dir_B		= HIGH;
					break;	
			// 좌회전
			case 'L':
					speed_A = 0.6;
					speed_B = 0.4;
					dir_A		= HIGH;
					dir_B		= LOW;
					break;
			// 직진 우회전
			case'I': 
					speed_A = 0.5;
					speed_B = 0.9;
					dir_A		= HIGH;
					dir_B		= HIGH;
					break;		
			// 직진 좌회전
			case'G': 
					speed_A = 0.9;
					speed_B = 0.5;
					dir_A		= HIGH;
					dir_B		= HIGH;
					break;
			// 정지
			case'S': 
					speed_A = 0;
					speed_B = 0;
					dir_A		= HIGH;
					dir_B		= HIGH;
					break;	
			}
		}
	}
}

void USART2_IRQHandler(){         //USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART1, &pcData, 1);
		//printf("Nucleo got : %c \r\rn", pcData);
		
		printf("%c", pcData);
		
		if (pcData == END_CHAR)
			printf("\r\n");
	}
}

// ultra sonic
// 초음파 원리
// trig에서 10us짜리 신호를 내가 설정한 주기마다 발생한다. -> 8번의 초음파 발사 전의 준비단계
// 8번의 초음파 발사한다. 
// echo에서 rise edge에서 한번 falling edge에서 한번 체크 한다 
// 이때 발생한 시간을 체크 한다. 

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		ovf_cnt++;														// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 3)){ 									// TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
		timeSt = TIM2->CCR3;									// Capture TimeStart
		clear_CCIF(TIM2, 3);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2, 4)){ 							// TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
		timeEnd = TIM2->CCR4;									// Capture TimeEnd
		clear_CCIF(TIM2, 4);								  // clear capture/compare interrupt flag 
		
		timeInterval = PulseWidth*((timeEnd - timeSt)+(ovf_cnt * 0xFFFF)); 					// Total time of echo pulse (10us * counter pulses -> [msec] unit)
		distance = (float) timeInterval / 58; 	// [mm] -> [cm]
		
		
		ovf_cnt = 0;                        	// overflow reset
		
	}	
}

//void EXTI15_10_IRQHandler(void){   // External Interrupt for pin 10 to 15
//	if(is_pending_EXTI(BUTTON_PIN)){ // check the pending's flag on 
//		
//		flag ^= 1;
//		
//		clear_pending_EXTI(BUTTON_PIN);
//	}
//}