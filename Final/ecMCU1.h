#ifndef __MY_ADC_H
#define __MY_ADC_H
#include "stm32f411xe.h"
#include "ecPWM.h"
#include "ecSysTick.h"
#include "ecGPIO.h"
#include "ecTIM.h"
#include "ecUART.h"
#include <stdint.h>

// servo 
#define SRD (float)(0.6) 		// SERVO_ROTATE_DUTY
#define SSD (float)(0.0) 			// SERVO_STOP_DUTY
#define SPP 20000 // 100[us] 	// SERVO_PWM_PERIOD

typedef struct{
	PWM_t *pwm1;
	GPIO_TypeDef *port1;
	int pin1;
	PWM_t *pwm2;
	GPIO_TypeDef *port2;
	int pin2;
	PWM_t *pwm3;
	GPIO_TypeDef *port3;
	int pin3;
} MCU1_t;

void MCU1_init(PWM_t *PWM1, GPIO_TypeDef* port1, int pin1, PWM_t *PWM2, GPIO_TypeDef* port2, int pin2, PWM_t *PWM3, GPIO_TypeDef* port3, int pin3);
void MCU1_Out(uint32_t state);
uint32_t MCU1_nextState(uint32_t state);

void SERVO_ONOFF(PWM_t *pwm, int ONOFF);

#endif
