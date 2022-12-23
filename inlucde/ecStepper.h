/*----------------------------------------------------------------\
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SeungEun Hwang
Created          : 09-13-2022
Modified         : 10-27-2022
Language/ver     : C++ in Keil uVision

Description      : HAL library ecStepper
/----------------------------------------------------------------*/
#include "ecInclude.h"

#ifndef __EC_STEPPER_H
#define __EC_STEPPER_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//State mode
#define HALF 0
#define FULL 1	

#define HALFSTEP 4096
#define FULLSTEP 2048	

#define GEAR_RATIO  32
#define FULL_REV  	64
#define HALF_REV  	128
 
//State number 
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4
#define S5 5
#define S6 6
#define S7 7

// direction 
#define CW 0 // 얘네 cw ccw 정해주기
#define CCW 1

//pin number
#define	 	PB10 	10
#define		PB5		5
#define		PB4		4
#define		PB3		3


/* Stepper Motor */
//stepper motor function

typedef struct{
   GPIO_TypeDef *port1;
   int pin1;
	 GPIO_TypeDef *port2;
   int pin2;
	 GPIO_TypeDef *port3;
   int pin3;
	 GPIO_TypeDef *port4;
   int pin4;
	 int _step_num;
} Stepper_t;

	 
void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4);
void Stepper_setSpeed (long whatSpeed, int mode);
void Stepper_pinOut (uint32_t state, int mode);
void Stepper_step(int steps, int dir, int mode, long rpm);

void Stepper_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
