/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  EC_HAL_for_student_exercise 
* 
******************************************************************************
*/

#include "ecInclude.h"

#ifndef __EC_PWM_H
#define __EC_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define UPPCOUNT		0
#define DOWCOUNT		1


typedef struct{
   GPIO_TypeDef *port;
   int pin;
   TIM_TypeDef *timer;
   int ch;
} PWM_t;


/* PWM Configuration */
void PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin, int speed ,int dir, uint32_t msec);
void PWM_period_ms(PWM_t *pwm,  uint32_t msec);		
void PWM_period_us(PWM_t *pwm, uint32_t usec);  


void PWM_pulsewidth_ms(PWM_t *pwm, float pulse_width_ms);
void PWM_duty(PWM_t *pwm, float duty);
void PWM_pinmap(PWM_t *PWM_pin);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
