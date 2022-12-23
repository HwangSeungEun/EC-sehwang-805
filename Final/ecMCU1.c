#include "stm32f411xe.h"
#include "ecMCU1.h"

//State number 
#define S0 0	// wait
#define S1 1	// Release product R
#define S2 2	// Release product G
#define S3 3	// Release product B
#define S4 4

volatile MCU1_t myPlant; 

typedef struct {
	uint8_t out[3];
  uint32_t next[4];
} State_MCU1_t;

State_MCU1_t FSM_MCU1[4] = {
	// RGBWO (R: Product R, G: Product G, B: Product B,W: print 'waiting for operating', O: print 'operating' )
	{{0,0,0},{S0,S1,S2,S3}}, // s0: wait
	{{1,0,0},{S4,S4,S4,S4}}, // s1: R
	{{0,1,0},{S4,S4,S4,S4}}, // s2: G
	{{0,0,1},{S4,S4,S4,S4}}, // s3: B
	{{0,0,0},{S4,S4,S4,S4}}  // s4: dagie
};

void MCU1_init(PWM_t *PWM1, GPIO_TypeDef* port1, int pin1, PWM_t *PWM2, GPIO_TypeDef* port2, int pin2, PWM_t *PWM3, GPIO_TypeDef* port3, int pin3){
	 
	myPlant.pwm1 	= PWM1;	
	myPlant.port1 = port1;
	myPlant.pin1  = pin1;	
	myPlant.pwm2 	= PWM2;	
	myPlant.port2 = port2;
	myPlant.pin2  = pin2;	
	myPlant.pwm3 	= PWM3;	
	myPlant.port3 = port3;
	myPlant.pin3  = pin3;	
	
	PWM_init_us(myPlant.pwm1, myPlant.port1, myPlant.pin1,SPP);
	PWM_duty(myPlant.pwm1,0.0);
	PWM_init_us(myPlant.pwm2, myPlant.port2, myPlant.pin2,SPP);
	PWM_duty(myPlant.pwm2,0.0);
	PWM_init_us(myPlant.pwm3, myPlant.port3, myPlant.pin3,SPP);
	PWM_duty(myPlant.pwm3,0.0);
}

void SERVO_ONOFF(PWM_t *pwm, int ONOFF){
	if(ONOFF == 1){PWM_duty(pwm,SRD);}
	else					{PWM_duty(pwm,0);}
}

void MCU1_Out(uint32_t state){
	SERVO_ONOFF(myPlant.pwm1, FSM_MCU1[state].out[0]);
	SERVO_ONOFF(myPlant.pwm2, FSM_MCU1[state].out[1]);
	SERVO_ONOFF(myPlant.pwm3, FSM_MCU1[state].out[2]);
	if			(state == 0){printf("\033[H\033[2J"); printf("RGB Input: "); delay_ms(1000);}
	else if	(state == 1){printf("\33[2K\r"); printf("\r\nProduct = RED"); delay_ms(7000);}
	else if	(state == 2){printf("\33[2K\r"); printf("\r\nProduct = GREEN"); delay_ms(7000);}
	else if	(state == 3){printf("\33[2K\r"); printf("\r\nProduct = BLUE"); delay_ms(7000);}
	else if	(state == 4){printf("\r\nWaiting for finishing delivery...\r\n"); delay_ms(1000);}
}

uint32_t MCU1_nextState(uint32_t state){
	return FSM_MCU1[state].next[state];
}
