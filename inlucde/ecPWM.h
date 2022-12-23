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
uint32_t is_TIM_ENA(PWM_t *pwm, GPIO_TypeDef *port, int pin); // TIM ENABLE check

void PWM_disable(PWM_t *pwm, GPIO_TypeDef *port, int pin);
void PWM_enable(PWM_t *pwm, GPIO_TypeDef *port, int pin);

void PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin, int pupd, int speed, int type ,int dir, uint32_t msec);
void PWM_period_ms(PWM_t *pwm,  uint32_t msec);		
void PWM_period_us(PWM_t *pwm, uint32_t usec);  
void PWM_pulsewidth_ms(PWM_t *pwm, float pulse_width_ms);
void PWM_pulsewidth_us(PWM_t *pwm, float pulse_width_us);
void PWM_duty(PWM_t *pwm, float duty);
void PWM_pinmap(PWM_t *PWM_pin);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
