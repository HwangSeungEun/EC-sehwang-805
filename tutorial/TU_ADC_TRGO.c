

/**
******************************************************************************
* @author  Hwang SeungEun
* @Mod		 2022-11-22 by Hwang SeungEun
* @brief   Embedded Controller:  Tutorial ADC Trigger 
*					 
* 
******************************************************************************
*/
#include "ecInclude.h"


static volatile double result_v =0;
void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
//// GPIO configuration ---------------------------------------------------------------------	
//// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
//	GPIO_init(GPIOA, 1, ANALOG);				// ANALOG = 3
//	GPIO_pupd(GPIOA, 1, NOPUPD);			// EC_NONE = 0
//	
//	
//	/* ADC_initiation --------------------------------------------------------------------*/
// // 1. Total time of conversion setting
//	// Enable ADC pheripheral clock	
//	RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN; 					// Enable the clock of RCC_APB2ENR_ADC1EN
//	
//	// Configure ADC clock pre-scaler
//	ADC->CCR &= 	~( 0b11 << 16);	// clear
//	ADC->CCR |= 0b00 << 16; // 00: PCLK2 divided by 2	(42MHz)
//	
//	// Configure ADC resolution 
//	ADC1->CR1 &= ~( 0b11 << 24); // clear
//	ADC1->CR1 |= 0b00 << 24; // 00: 12-bit resolution (15cycle+)
//	
//	// Configure channel sampling time of conversion.	
//	// Software is allowed to write these bits only when ADSTART=0 and JADSTART=0	!!
//	// ADC clock cycles @42MHz = 2us
//	ADC1->SMPR2 &= ~(7  << 3);		// Sampling Time:  clear bits
//	ADC1->SMPR2 |= 0b100 << 3;		// Sampling Time:  84cycles/42Mhz=2us
//	   
//// 2. Regular / Injection Group 
//	//Regular: SQRx, Injection: JSQx
//	
//// 3. Repetition: Single or Continuous conversion
//	ADC1->CR2 |= ADC_CR2_CONT;	// Enable Continuous conversion mode	
// 
//// 4. Single Channel or Scan mode
//	// Configure the sequence length
//	ADC1->SQR1 &= ~(0b1111 << 20);	  // clear
//	ADC1->SQR1 |= (0b0000 << 20);		// 0000: 1 conversion in the regular channel conversion sequence
//	// Configure the channel sequence 
//	ADC1->SQR3 &= ~ (0x1F << 0);	// Choose CH of 1st Conversion: clear bits
//	ADC1->SQR3 |= 0b0001 << 0 ;// Choose CH of 1st Conversion: Select CH (0~15)	
//	
//	//  - Single Channel: scan mode, right alignment	
//	ADC1->CR1 |= 	ADC_CR1_SCAN_Msk;// 1: Scan mode enable 
//	ADC1->CR2 &= ~ADC_CR2_ALIGN; // 0: Right alignment	
//	
//// 5. Interrupt Enable
//	// Enable EOC(conversion) interrupt. 
//	ADC1->CR1 &= ~(1<<5);         			// Interrupt reset
//	ADC1->CR1 |= (1<<5);         			// Interrupt enable
//	
//	// Enable ADC_IRQn 
//	NVIC_SetPriority (ADC_IRQn, 2);	// Set Priority to 2
//	NVIC_EnableIRQ(ADC_IRQn);      	// Enable interrupt form ACD1 peripheral	

//// 6. Enable ADC
//	ADC1->CR2 |= ADC_CR2_ADON;

//  
//	
//	//Single conversion mode
//	ADC1->CR2 &= ~ADC_CR2_CONT;   // Discontinuous conversion mode
//	ADC1->CR2 |= ADC_CR2_EOCS;  	// Enable EOCS
//	
//// HW Trigger configuration -------------------------------------------------------------
//// 1. TIMx Trigger Output Config
//	// Enable TIMx Clock
//	TIM_TypeDef *TIMx = TIM3;			// Default TRGO : TIM3
//	int msec = 1;									// Default msec : 1 msec
//	TIM_init(TIMx, msec);					// TIM3 init (use user defined HAL)
//	TIMx->CR1 &= ~1; 							// counter disable
//	
//	// Set PSC, ARR
//  TIM_period_ms(TIMx, msec);
//	
//  // Master Mode Selection MMS[2:0]: Trigger output (TRGO)
//  TIMx->CR2 &= 	~TIM_CR2_MMS;						// reset MMS
//  TIMx->CR2 |=   TIM_CR2_MMS_2;					//100: Compare - OC1REF signal is used as trigger output (TRGO)
//   
//	// Output Compare Mode
//  TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;     										// OC1M : output compare 1 Mode 
//  TIMx->CCMR1 |=  (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);	// OC1M = 110 for compare 1 Mode ch1 
//	
//  // OC1 signal 
//  TIMx->CCER |= TIM_CCER_CC1E;					// CC1E Capture enabled
//	TIMx->CCR1 =  (TIMx->ARR)/2;					// CCR set
//   
//  // Enable TIMx 
//  TIMx->CR1 |= TIM_CR1_CEN; 						//counter enable
//	
//// 2. HW Trigger
//	// Select Trigger Source
//	ADC1->CR2 &= ~ADC_CR2_EXTSEL; 				// reset EXTSEL
//	ADC1->CR2 |=  ADC_CR2_EXTSEL_3;				// TIMx TRGO event (ADC : TIM2, TIM3 TRGO) default : TIM3
//	
//	//Select Trigger Polarity 
//	ADC1->CR2 &= 	~ADC_CR2_EXTEN;					// reset EXTEN, default
//	ADC1->CR2 |= 	ADC_CR2_EXTEN_1;				// trigger detection rising edge
//	
//	// Enable ADC 
//	ADC1->CR2 |= ADC_CR2_ADON;
//	
//  // ADC Start (SW, once)	  
//  ADC1->CR2 |= ADC_CR2_SWSTART;

	/* Inifinite Loop --------------------------------------------------------------------*/
	while(1);
}

// Initialiization 
void setup(void){	
	RCC_PLL_init();                 // System Clock = 84MHz
	UART2_init();
	SysTick_ms_init(1);
	ADC_init(GPIOA,1,SW);
	ADC_TRGO(GPIOA, 1, FALL);
	ADC_start();
}


void ADC_IRQHandler(void){		
	if((ADC1->SR & ADC_SR_OVR) == ADC_SR_OVR){
		ADC1->SR &= ~ADC_SR_OVR;
	}
	
	if(is_ADC_EOC){       			
	  result_v = ADC1->DR;
		printf("voltage = %.3f\r\n",result_v*3.3/4095);
 }
}