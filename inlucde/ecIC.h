
#include "ecInclude.h"


#ifndef __EC_IC_H
#define __EC_IC_H



#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/* Input Capture*/

// Edge Type
#define IC_RISE 0
#define IC_FALL 1
#define IC_BOTH 2


//Input Capture
typedef struct{
	GPIO_TypeDef *port;
	int pin;   
	TIM_TypeDef *timer;
	int ch;  		//int Timer Channel
	int ICnum;  //int IC number
} IC_t;




void ICAP_init(IC_t *ICx, GPIO_TypeDef *port, int pin);
void ICAP_setup(IC_t *ICx, int IC_number, int edge_type);
void ICAP_counter_us(IC_t *ICx, int usec);

uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum);
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum);

void ICAP_pinmap(IC_t *timer_pin);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
