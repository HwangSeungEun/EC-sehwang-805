#include "ecInclude.h"

#define LED_PIN 5
#define END_CHAR 13
#define A 0
#define B 1

#define PIEZO_PIN 	1
#define ILLUM_PIN		0
#define IR_R_PIN 		0
#define IR_L_PIN 		1

#define TRIG_PIN		6		
#define ECHO_PIN		10		

#define PWM_PERIOD	100

#define ILLUML_CUT  1100

#define DELAY_MSEC  20

#define DELIVERY_MSEC 5000
#define STOP_MSEC			10000

//#define MAX_BU


/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
int seqCHn[4] = {8,9,10,  }; // chn number

uint32_t IR1 = 0;
uint32_t IR2 = 0;
uint32_t ILLUM = 0;
uint32_t PIEZO = 1; //piezoelectric sensor

// flag 
int flag = 0;
int bluetooth = 0;

// rc카 속도 및 모터 회전 방향 설정
float 	speed_A = 0.0;
float 	speed_B = 0.0;

int			dir_A 	= HIGH;
int 		dir_B 	= HIGH;

uint32_t ovf_cnt 		= 0;
float distance			= 0.0;
float timeInterval 	= 0.0;
float timeSt 				= 0.0;
float timeEnd 			= 0.0;

float PulseWidth 		= 10;


typedef struct {
		GPIO_TypeDef *port;
		int pin; // TIM3 Ch3
}_Pin;

_Pin dcPwmPin[2] = {
	{GPIOA, 8}, // TIM1 ch1
	{GPIOA, 9}	// TIM1 ch2
};

_Pin dcDirPin[2] = {
	{GPIOC, 9}, 	//모터 A
	{GPIOC, 8}		//모터 B
};

PWM_t dcPwm[2];

enum Num {STOP = 0, 
					DERIVE_1, 
					DELIVERY, 
					DERIVE_2,
					ULTRASONIC
}MODE;

int preMODE = 99;


void setup(void);
void EXTI15_10_IRQHandler(void);
void ADC_IRQHandler(void);
	
int main(void) { 
	
	setup();
	MODE = STOP;	
	printf("strat! \r\n");
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
//		printf("PIEZO = %d\r\n",GPIO_read(GPIOA, 0));	
		printf("IR1 = %d \r\n",IR1); // right
		printf("IR2 = %d \r\n",IR2); // left  
		printf("ILLUM = %d \r\n",ILLUM); 			
		printf("dis = %f \r\n",(double)distance); 
		printf("PIEZO = %d \r\n",PIEZO); // 
		printf("preMODE = %d \r\n", preMODE);
		printf("MODE = %d \r\n", MODE);
		printf("\r\n"); 
		
		PIEZO = GPIO_read(GPIOC, PIEZO_PIN);			
		
		// state를 전부다 쪼개본다 
		switch(MODE){
			
		// 처음 정지
		case STOP:	
			PWM_disable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
			PWM_disable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
			
			if(PIEZO == 0 && is_TIM_ENA(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin) == 0) {
				preMODE = MODE;
				MODE = DERIVE_1;
					
				PWM_enable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
				PWM_enable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
			}
			break;
			
		// 배달 전 주행
		case DERIVE_1:		
			PWM_duty(&dcPwm[A], 1.0 - speed_A);
			PWM_duty(&dcPwm[B], 1.0 - speed_B );
			GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
			GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진	


			if(ILLUM <ILLUML_CUT && PIEZO == 0){
				preMODE = MODE;
				MODE = DELIVERY;										
			}
			break;
			
		// 배달 도착
		case DELIVERY:
			PWM_disable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
			PWM_disable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
	
				
			if(ILLUM > ILLUML_CUT && preMODE == DERIVE_1){
				
//				delay_ms(DELIVERY_MSEC);			// 임시로 5초 정지
				preMODE = MODE;
				MODE = DERIVE_2;
				PWM_enable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
				PWM_enable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);

			}
			break;

		// 배달 후 주행
		case DERIVE_2:
			PWM_duty(&dcPwm[A], 1.0 - speed_A);
			PWM_duty(&dcPwm[B], 1.0 - speed_B );
			GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
			GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진		
		
			if((distance > 2 && distance < 15)){
					preMODE = MODE;
					MODE = ULTRASONIC;
			}
			break;

		// 다시 처음 정지
		case ULTRASONIC:	
			PWM_disable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
			PWM_disable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);	
			delay_ms(STOP_MSEC);
			preMODE = MODE;			
			MODE = STOP;
			break;
		}	
		
		delay_ms(DELAY_MSEC);
	}
}
// Initialiization  
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_ms_init(1);
	USART_init(USART2, 9600);
	
	// internal LED set
	GPIO_out_set(GPIOA, LED_PIN, PULLUP, MSPEED, PUSHPULL);
	
	// Piezo sensor
	GPIO_in_set(GPIOC, PIEZO_PIN, PULLUP);
	
	// IR sensor
	ADC_INJ_init(GPIOB, IR_R_PIN, TRGO);
	ADC_INJ_init(GPIOB, IR_L_PIN, TRGO);
	
	// Illimination sensor
	ADC_INJ_init(GPIOC, ILLUM_PIN, TRGO);
	
	// ADC injection sequence
	ADC_INJ_sequence(3, seqCHn); 
	ADC_INJ_start();
	
	// pwm dir setting
	for (int i = 0; i < 2; i++){
		GPIO_out_set(dcDirPin[i].port, dcDirPin[i].pin, PULLDO,HSPEED,PUSHPULL);
	}
	
	// PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin, int pupd, int speed, int type ,int dir, uint32_t msec);
	PWM_init(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin, FSPEED, PULLDO, PUSHPULL, UPPCOUNT,1);
	PWM_init(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin, FSPEED, PULLDO, PUSHPULL, UPPCOUNT,1);
	PWM_period_us(&dcPwm[A], PWM_PERIOD);
	PWM_period_us(&dcPwm[B], PWM_PERIOD);
	
	// speed control 
	PWM_duty(&dcPwm[A], 1 - speed_A);
	PWM_duty(&dcPwm[B], 1 - speed_B );
	
	// A는 HIGH일 때 전진
	GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, HIGH);
	// B는 HIGH일 때 전진
	GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, HIGH);
	

	
	// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;												// PWM1 for trig
	PWM_init(&trig, GPIOA, TRIG_PIN, NOPUPD, FSPEED, PUSHPULL, UPPCOUNT, 1);	// PA_6: Ultrasonic trig pulse TIM3 ch1
	PWM_period_us(&trig, 50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig, 10);   	// PWM pulse width of 10us
	
	// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;												// Input Capture for echo
	ICAP_init(&echo, GPIOB, ECHO_PIN);   	// PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, IC_RISE);   	// TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, IC_FALL);   	// TIM2_CH3 as IC4 , falling edge detect	
	
	// NVIC Priority setting
	NVIC_SetPriority(ADC_IRQn, 2);						    	
	NVIC_EnableIRQ(ADC_IRQn);									  
	
	NVIC_SetPriority(TIM2_IRQn, 1);		// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);				// TIM2 interrupt request enable
	
	
}

void ADC_IRQHandler(void){
	
	if(is_ADC_JEOC()){       //after finishing sequence
		
		IR1 = ADC1->JDR3;
		IR2 = ADC1->JDR2;
		ILLUM = ADC1->JDR1;
		
		clear_ADC_JEOC();
	}
	

	if (IR1 > 2000 && IR2 < 2000) { 
		// 직진 우회전
		speed_A = 0.95;
		speed_B = 0.6;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}			
	else if (IR2 > 2000 && IR1 < 2000) {
		// 직진 좌회전
		speed_A = 0.6;
		speed_B = 0.95 ;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}else {
		speed_A = 0.7;
		speed_B = 0.7;  
		dir_A		= HIGH;
		dir_B		= HIGH;
	}
		
	if(ILLUM <ILLUML_CUT && MODE == DERIVE_1){
		speed_A = 0.0;
		speed_B = 0.0;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}
	
	if((distance > 2 && distance < 15) &&  MODE == DERIVE_2){
		speed_A = 0.0;
		speed_B = 0.0;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}

}

// ultra sonic
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


