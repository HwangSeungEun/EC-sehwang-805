/**
******************************************************************************
* @author  SeungEun Hwang 
* @Mod		 2022-10-25 by SeungEun Hwang  	
* @brief   Embedded Controller:  PWM header file
* 
******************************************************************************
*/


#include "ecInclude.h"

/* PWM Configuration */

void PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin, int speed ,int dir, uint32_t msec){
// 0. Match Output Port and Pin for TIMx 	
		pwm->port = port;
		pwm->pin  = pin;
		PWM_pinmap(pwm);
		TIM_TypeDef* TIMx = pwm->timer;
		int CHn = pwm->ch;
	//PWM_pinmap(pwm) 여기서 TIMx와 ch를 골라준다 내가 집어넣은 값고 맞게 타이머와 채널이 설정된다
	// AF1 at PA5 = TIM2_CH1 (p.150)  타이머마다 AF값이 다르다 
	//-----------------------------------------------------
	/*	
				TIM	Ch 	Por	pin
				1		1		A		8
				1		1n	A		7
				1		1n	B		13
						
				1		2		A		9
				1		2n	B		0
				1		2n	B		14
						
				1		3		A		10
				1		3n	B		11
				1		3n	B		15

				2		1		A		0
				2		1		A		5
				2		1		A		15

				2		2		A		1
				2		2		B		3

				2		3		B		10

				3		1		A		6
				3		1		B		4
				3		1		C		6

				3		2		B		5
				3		2		C		7
				
				3		4		C		9

				4		1		B		6
				4		2		B		7
				4		3		B		8
				4		4		B		9
			.... 
			이렇게 이미 다 지정이 되어 있다. 근데 그건 
			PWM_pinmap(pwm);여기서 해준다
	*/
	//-----------------------------------------------------	


// 1. Initialize GPIO port and pin as AF
	GPIO_AF_set(port, pin, PULLUP, speed, PUSHPULL);
	
// 2. Configure GPIO AFR by Pin num.		
	// pwm 채널에 따른 일반화 필요
	// AFR[0] == AFRL,   AFR[1] == AFRH 로 구분짓기 위해 배열을 사용했다. 4bit 16개가 필요하기 때문에 64bit
	// pin 0 ~ 7까지 AFR[0], pin 8 ~ 15까지 AFR[1]
	// bit wise 할 때는 0~15번 pin이기 때문에 8개씩 나뉘고 0~7씩 bit연산을 하기 때문에 %계산을 했다.
	
	// AF1: TIM1,2		AF2: TIM3~5			AF3: TIM9~11
	// bit 연산자는 다 되어있다
	// 일단 타이머 별로 다 AF를 구분해 주는 if문을 정리하자 
	
	uint8_t AFx = 0;
	if ((TIMx == TIM1) || (TIMx == TIM2)) { AFx = 1UL;}
	else if ((TIMx == TIM3) || (TIMx == TIM4) || (TIMx == TIM5)) { AFx = 2UL; }
	else if ((TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11)) { AFx = 3UL; }

	// 각 핀별로 AFR 배열로 들어가는 일반화를 만들자
	// AFR[0] for pin: 0~7		AFR[1] for pin:8~15
	port->AFR[pin/8]	 &= ~(0xFUL	<< (4*(pin%8)));			// 4 bit clear AFRx
	port->AFR[pin/8]	 |= 	AFx 	<< (4*(pin%8));          						

	
// 3. Initialize Timer 
		TIM_init(TIMx, msec);	// with default msec=1 value.		
		TIMx->CR1 &= ~TIM_CR1_CEN;	// disable counter
// 3-2. Direction of Counter
		
		// Counting direction: 0 = up-counting, 1 = down-counting
		if(dir == UPPCOUNT){		TIMx->CR1 &= ~TIM_CR1_DIR_Msk;}
		else if(dir == DOWCOUNT){	TIMx->CR1 |= TIM_CR1_DIR_Msk;}
			
// 4. Configure Timer Output mode as PWM
	uint32_t ccVal=TIMx->ARR/2;  // default value  CC=ARR/2
	// 몇%로 내가 CCR을 해서 pwm의 비율을 늘릴지 입력해주는거 필요하다 	ccVal=TIMx->ARR *(m / n) 대충 이런느낌으로
		
	if(CHn == 1){
		TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;                     // Clear ouput compare mode bits for channel 1
		TIMx->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;	// OC1M = 110 for PWM Mode 1 output on ch1. #define TIM_CCMR1_OC1M_1          (0x2UL << TIM_CCMR1_OC1M_Pos)
		TIMx->CCMR1	|= TIM_CCMR1_OC1PE;                     // Output 1 preload enable (make CCR1 value changable)
		
		// 여기는 ccr 값을 정해준다 내가 밖에서 지정을 해줘야할 부분일듯 
		TIMx->CCR1  = ccVal; 																// Output Compare Register for channel 1 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC1P;                       // select output polarity: active high	
		TIMx->CCER  |= TIM_CCER_CC1E;												// Enable output for ch1
	}
	else if(CHn == 2){
		TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;                     // Clear ouput compare mode bits for channel 2
		TIMx->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;	// OC1M = 110 for PWM Mode 1 output on ch2
		TIMx->CCMR1	|= TIM_CCMR1_OC2PE;            					// Output 1 preload enable (make CCR2 value changable)	
		TIMx->CCR2  = ccVal;	 															// Output Compare Register for channel 2 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC2P;             					// select output polarity: active high	
		TIMx->CCER  |= TIM_CCER_CC2E;												// Enable output for ch2
	}
	else if(CHn == 3){
		TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;                     // Clear ouput compare mode bits for channel 3
		TIMx->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;	// OC1M = 110 for PWM Mode 1 output on ch3
		TIMx->CCMR2	|= TIM_CCMR2_OC3PE;            					// Output 1 preload enable (make CCR3 value changable)	
		TIMx->CCR3  = ccVal;	 															// Output Compare Register for channel 3 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC3P;             					// select output polarity: active high	
		TIMx->CCER  |= TIM_CCER_CC3E;												// Enable output for ch3
	}
	else if(CHn == 4){

		TIMx->CCMR2 &= ~TIM_CCMR2_OC4M;
		TIMx->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
		TIMx->CCMR2 |= TIM_CCMR2_OC4PE;
		TIMx->CCR4 = ccVal;
		TIMx->CCER &= ~TIM_CCER_CC4P;
		TIMx->CCER |= TIM_CCER_CC4E;
	}	

// 5. Enable Timer Counter ??? 원래 TIMx == TIM1만 되어있었다 이거는 뭐하는 놈일까???
	if(TIMx == TIM1) {
		TIMx->BDTR |= TIM_BDTR_MOE;					// Main output enable (MOE): 0 = Disable, 1 = Enable	
	}		

	TIMx->CR1 |= TIM_CR1_CEN;
	// Enable counter
}



void PWM_period_ms(PWM_t *pwm, uint32_t msec){
	TIM_TypeDef *TIMx = pwm->timer;
	TIM_period_ms(TIMx, msec);  
}


void PWM_period_us(PWM_t *pwm, uint32_t usec){
	TIM_TypeDef *TIMx = pwm->timer;
	TIM_period_us(TIMx, usec); 
}


void PWM_pulsewidth_ms(PWM_t *pwm, float pulse_width_ms){ 
	int CHn = pwm->ch;
	uint32_t fsys = 0;
	TIM_TypeDef *TIMx = pwm->timer;
	uint32_t psc = TIMx->PSC;
	
	// Check System CLK: PLL or HSI
	//PLL
	if((RCC->CFGR & (3<<0)) == 2)      { fsys = 84000; }  // for msec 84MHz/1000
	// HSI
	else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16000; }
	
	
	////////----------------------
	// 이거 자료형이 안맞는거 같은데?
	float fclk = fsys / (psc + 1);					// fclk=fsys/(psc+1);
	uint32_t ccval = pulse_width_ms * fclk - 1.0;		// pulse_width_ms *fclk - 1;
	////////----------------------
	
	switch(CHn){
		case 1: pwm->timer->CCR1 = ccval; break;
		case 2: pwm->timer->CCR2 = ccval; break;
		case 3: pwm->timer->CCR3 = ccval; break;
		case 4: pwm->timer->CCR4 = ccval; break;

		default: break;
	}
}


void PWM_duty(PWM_t *pwm, float duty) {                 //  duty=0 to 1	 한 주기 안에서 duty는 0에서 1사이다
	// ccval은 arr을 도달할 때 내가 지정한 값을 지나가면 그때 HIGH가 되게하는거다
	TIM_TypeDef *TIMx = pwm->timer;
	uint32_t arr = TIMx->ARR;
	
	float ccval = ((float)(arr + (uint32_t)1) ) * duty ;    			// (ARR+1)*dutyRatio         
	int CHn = pwm->ch;
  	
	//YOUR CODE GOES HERE
	switch(CHn){
		case 1: TIMx->CCR1 = ccval; break;
		case 2: TIMx->CCR2 = ccval; break;
		case 3: TIMx->CCR3 = ccval; break;
		case 4: TIMx->CCR4 = ccval; break;
		default: break;
	}
}


// DO NOT MODIFY HERE
void PWM_pinmap(PWM_t *pwm){
   GPIO_TypeDef *port = pwm->port;
   int pin = pwm->pin;
   
   if(port == GPIOA) {
      switch(pin){
         case 0 : pwm->timer = TIM2; pwm->ch = 1; break;
         case 1 : pwm->timer = TIM2; pwm->ch = 2; break;
         case 5 : pwm->timer = TIM2; pwm->ch = 1; break;
         case 6 : pwm->timer = TIM3; pwm->ch = 1; break;
         //case 7: PWM_pin->timer = TIM1; PWM_pin->ch = 1N; break;
         case 8 : pwm->timer = TIM1; pwm->ch = 1; break;
         case 9 : pwm->timer = TIM1; pwm->ch = 2; break;
         case 10: pwm->timer = TIM1; pwm->ch = 3; break;
         case 15: pwm->timer = TIM2; pwm->ch = 1; break;
         default: break;
      }         
   }
   else if(port == GPIOB) {
      switch(pin){
         //case 0: PWM_pin->timer = TIM1; PWM_pin->ch = 2N; break;
         //case 1: PWM_pin->timer = TIM1; PWM_pin->ch = 3N; break;
         case 3 : pwm->timer = TIM2; pwm->ch = 2; break;
         case 4 : pwm->timer = TIM3; pwm->ch = 1; break;
         case 5 : pwm->timer = TIM3; pwm->ch = 2; break;
         case 6 : pwm->timer = TIM4; pwm->ch = 1; break;
         case 7 : pwm->timer = TIM4; pwm->ch = 2; break;
         case 8 : pwm->timer = TIM4; pwm->ch = 3; break;
         case 9 : pwm->timer = TIM4; pwm->ch = 4; break;
         case 10: pwm->timer = TIM2; pwm->ch = 3; break;
         
         default: break;
      }
   }
   else if(port == GPIOC) {
      switch(pin){
         case 6 : pwm->timer = TIM3; pwm->ch = 1; break;
         case 7 : pwm->timer = TIM3; pwm->ch = 2; break;
         case 8 : pwm->timer = TIM3; pwm->ch = 3; break;
         case 9 : pwm->timer = TIM3; pwm->ch = 4; break;
         
         default: break;
      }
   }
	 // TIM5 needs to be added, if used.
}
