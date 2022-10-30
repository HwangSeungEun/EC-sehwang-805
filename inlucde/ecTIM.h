/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  EC_HAL_for_student_exercise 
* 
******************************************************************************
*/

#ifndef __EC_TIM_H 
#define __EC_TIM_H

#include "ecInclude.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/* Timer Configuration */
void TIM_init(TIM_TypeDef *TIMx, uint32_t msec);  
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec);  
void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec);

void TIM_INT_init(TIM_TypeDef* TIMx, uint32_t msec); 
void TIM_INT_enable(TIM_TypeDef* TIMx);
void TIM_INT_disable(TIM_TypeDef* TIMx);
void reset_TIMER(TIM_TypeDef *TIMx);


uint32_t is_pending_TIM(TIM_TypeDef *TIMx);
void clear_pending_TIM(TIM_TypeDef *TIMx);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
