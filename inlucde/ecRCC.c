/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SSS LAB
Created          : 05-03-2021
Modified         : 09-29-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

//#include "stm32f4xx.h"
//#include "ecRCC.h"

#include "ecInclude.h"



volatile int EC_SYSCLK=16000000;

void RCC_HSI_init() {
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  //RCC->CR |= ((uint32_t)RCC_CR_HSION);
	//	RCC->CR |= RCC_CR_HSION; 이거로 해되 되는거면 이거로 바꿔야겠다
	RCC->CR |= 0x00000001U;
	
  // wait until HSI is ready
  //while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	while ( (RCC->CR & 0x00000002U) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 								// not essential
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI; 								//00: HSI16 oscillator used as system clock

	
	// Wait till HSI is used as system clock source
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0 );

		EC_SYSCLK = MCU_CLK_HSI;
}



void RCC_HSE_init(){
	
	// HSE on
	RCC->CR |= RCC_CR_HSEON;
	
	// wait until HSE is ready
	while ( (RCC->CR & RCC_CR_HSERDY) == 0 ) {;}
		
	// Select HSE as system clock source 
  RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 							
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE; 								
		
		// Wait till HSE is used as system clock source
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 1 );
		   
	//EC_SYSTEM_CLK=16000000;
	//EC_SYSCLK=16000000;
	EC_SYSCLK = MCU_CLK_HSE;
	
	
}

void exam_RCC_PLL_init(int CLKSOURCE, int divM, int multN, int divP) {   
   // To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  // (HCLK) and the supply voltage of the device.      
   FLASH->ACR &= ~FLASH_ACR_LATENCY;
   FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
      
   // Enable the HSI, HSE oscillator
   if(CLKSOURCE == HSI){
      RCC->CR |= RCC_CR_HSION;
      while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);
   }
  else if(CLKSOURCE == HSE){
      RCC->CR |= RCC_CR_HSEON;
      while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);
   }
   
   // Disable PLL for configuration
   RCC->CR    &= ~RCC_CR_PLLON;
   
   // Select clock source to PLL
   RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;       // Set source for PLL: clear bits
   if(CLKSOURCE == HSI){
      RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // Set source for PLL: 0 =HSI, 1 = HSE
   }
  else if(CLKSOURCE == HSE){
      RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // Set source for PLL: 0 =HSI, 1 = HSE
   }
   // Make PLL as 84 MHz
   
   // CLKSOURCE
   // f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 84/8 = 168 MHz
   // f(PLL_R) = f(VCO clock) / PLLP = 168MHz/2 = 84MHz
   RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | multN << RCC_PLLCFGR_PLLN_Pos;
   RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | divM << RCC_PLLCFGR_PLLM_Pos; 
   RCC->PLLCFGR =(RCC->PLLCFGR & ~RCC_PLLCFGR_PLLP) | divP << RCC_PLLCFGR_PLLP_Pos ;  // 00: PLLP = 2, 01: PLLP = 4, 10: PLLP = 6, 11: PLLP = 8   
   
   
   // Enable PLL after configuration
   RCC->CR   |= RCC_CR_PLLON; 
   while((RCC->CR & RCC_CR_PLLRDY)>>RCC_CR_PLLRDY_Pos == 0);
   
   // Select PLL as system clock
   RCC->CFGR &= ~RCC_CFGR_SW;
   RCC->CFGR |= RCC_CFGR_SW_PLL;
   
   // Wait until System Clock has been selected
   while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
   
   // The maximum frequency of the AHB and APB2 is 100MHz,
   // The maximum frequency of the APB1 is 50 MHz.
   RCC->CFGR &= ~RCC_CFGR_HPRE;        // AHB prescaler = 1; SYSCLK not divided (84MHz)
   RCC->CFGR &= ~RCC_CFGR_PPRE1;       // APB high-speed prescaler (APB1) = 2, HCLK divided by 2 (42MHz)
   RCC->CFGR |=  RCC_CFGR_PPRE1_2;
   RCC->CFGR &= ~RCC_CFGR_PPRE2;       // APB high-speed prescaler (APB2) = 1, HCLK not divided   (84MHz)
   
   EC_SYSCLK = MCU_CLK_PLL;
}








void RCC_PLL_init() {	
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  // (HCLK) and the supply voltage of the device.		
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	// Enable the Internal High Speed oscillator (HSI)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	// 		(HSI on: 1& 1 << 1)   1  == 0 -->  조건에 맞지 않아서 탈출
	
	// Disable PLL for configuration
	RCC->CR    &= ~RCC_CR_PLLON;

	
		// Select clock source to PLL
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; 		// Set source for PLL: clear bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // Set source for PLL: 0 =HSI, 1 = HSE
		
	

		// Make PLL as 84 MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 84/8 = 168 MHz
	// f(PLL_R) = f(VCO clock) / PLLP = 168MHz/2 = 84MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 84U << 6;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 8U ; 
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;  // 00: PLLP = 2, 01: PLLP = 4, 10: PLLP = 6, 11: PLLP = 8	
	
		// Enable PLL after configuration
	RCC->CR   |= RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY)>>25 != 0); ///??????
	//     if (1		&  (1 << 25)) >> 25 != 0 이면 조건을 만족하기 때문에 while(1)이 된다 

		// Select PLL as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	


	
		// Wait until System Clock has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != 8UL);
	


	
	// The maximum frequency of the AHB and APB2 is 100MHz,
	// The maximum frequency of the APB1 is 50 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  		// AHB prescaler = 1; SYSCLK not divided (84MHz)
	RCC->CFGR &= ~RCC_CFGR_PPRE1; 		
	RCC->CFGR |=  RCC_CFGR_PPRE1_2;		// APB high-speed prescaler (APB1) = 2, HCLK divided by 2 (42MHz)
	RCC->CFGR &= ~RCC_CFGR_PPRE2; 		// APB high-speed prescaler (APB2) = 1, HCLK not divided	(84MHz)
	
	EC_SYSCLK=84000000;
}



void RCC_PLL_HSE_init() {	
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  // (HCLK) and the supply voltage of the device.		
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	// Enable the Internal High Speed oscillator (HSI)
	RCC->CR |= RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY) == 0);
	
	// Disable PLL for configuration
	RCC->CR    &= ~RCC_CR_PLLON;
	
	// Select clock source to PLL
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; 		// Set source for PLL: clear bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // Set source for PLL: 0 =HSI, 1 = HSE
	
	// Make PLL as 84 MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 8MHz * 84/4 = 168 MHz
	// f(PLL_R) = f(VCO clock) / PLLP = 168MHz/2 = 84MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 84U << 6;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 4U ; 
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;  // 00: PLLP = 2, 01: PLLP = 4, 10: PLLP = 6, 11: PLLP = 8	
	
	// Enable PLL after configuration
	RCC->CR   |= RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY)>>25 != 0);
	
	// Select PLL as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	// Wait until System Clock has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != 8UL);
	
	// The maximum frequency of the AHB and APB2 is 100MHz,
	// The maximum frequency of the APB1 is 50 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  		// AHB prescaler = 1; SYSCLK not divided (84MHz)
	RCC->CFGR &= ~RCC_CFGR_PPRE1; 		
	RCC->CFGR |=  RCC_CFGR_PPRE1_2;		// APB high-speed prescaler (APB1) = 2, HCLK divided by 2 (42MHz)
	RCC->CFGR &= ~RCC_CFGR_PPRE2; 		// APB high-speed prescaler (APB2) = 1, HCLK not divided	(84MHz)
	
	EC_SYSCLK=84000000;
}




void RCC_GPIOA_enable()
{
	// HSI is used as system clock         
	//	RCC_HSI_init();
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

void RCC_GPIOB_enable()
{
	// HSI is used as system clock         
//	RCC_HSI_init();
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}

void RCC_GPIOC_enable()
{
	// HSI is used as system clock         
//	RCC_HSI_init();
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

void RCC_GPIOD_enable()
{
	// HSI is used as system clock         
//	RCC_HSI_init();
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}

void RCC_GPIOE_enable()
{
	// HSI is used as system clock         
//	RCC_HSI_init();
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
}


void RCC_GPIOH_enable()
{
	// HSI is used as system clock         
//	RCC_HSI_init();
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
}
