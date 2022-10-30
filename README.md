# Embedded Controller 



**Date:** 2022-10-15

**Author**: SeungEung Hwang

**Github:** [repository link](https://github.com/HwangSeungEun/EC-sehwang-805)

**Program:** C/C++

**IDE/Compiler:** Keil uVision 5

**OS:** WIn10

**MCU:**  STM32F411RE (Nucleo-64)



## **Contents**

[TOC]

## **STM 32 Circuit**

![stm32](https://user-images.githubusercontent.com/91474647/196760439-ef17cb83-ba8d-4fd5-ba03-2d9ee3df5ffb.jpg)

## **Bitwise**

**a**: array of register, **k**: bit number

- **write a (high) bit:** a|= (1 << k)
- **write two bits:** a |= (3 << k)   
- **read one bit:** val = (a >> k) & 1
- **read 2 bits**: val = (a >> k) & 3   [0011]  
- **clear bit:** a &= ~(1 << k)
- **Toggle:** a^= 1 << k  (^= is XOR)





## **RCC**

### RCC Register Table



#### RCC_CR

![image](https://user-images.githubusercontent.com/91474647/196455006-24f68444-3f2f-4c61-85f7-c7f5d52413da.png)



#### RCC_PLLCFGR

![image](https://user-images.githubusercontent.com/91474647/196454878-c71a7d6e-fa41-49a8-abe2-0ac49d6869d4.png)



#### RCC_CFGR

![image](https://user-images.githubusercontent.com/91474647/196455684-4778e24a-392d-4745-8760-210858786b3b.png)



#### RCC_AHB1ENR

![image](https://user-images.githubusercontent.com/91474647/196472676-f2ae4781-10ae-405e-a483-6d5fd29a3a20.png)



#### RCC_APB1ENR

![image](https://user-images.githubusercontent.com/91474647/196473046-d0209d14-1430-48b3-844a-aaf29dcbb077.png)



#### RCC_APB2ENR

![image](https://user-images.githubusercontent.com/91474647/196473548-f090e23b-66ef-4ed8-9f35-c42a97831502.png)



### Internal Clock (HSI) for GPIO

1. **Enable HSI and choose as SYSCLK source**
   - Enable HSI: **(RCC->CR: HSION=1)** 
   - Wait until HSI is stable and ready: **(RCC->CR: HSIRDY? 1)** 
   - Choose the system clock switch : **(RCC->CFGR: SW = 00)** 
   - Check if the selected source is correct: **(RCC->CFGR: SWS ? 00)**

2. Configure HSI (optional) 
   - Calibration for RC oscillator: (RCC->CR: HSICAL, HSITRIM) 
3. Configure APB/AHB Prescaler (optional) 
   - Change Prescaler: RCC->CFGR: HPRE, PPRE

4. **Enable GPIOx clock(AHB1ENR )** 
   - Enable (RCC_AHB1ENR) for PORTx

```c
void RCC_HSI_init() {
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  	//RCC->CR |= ((uint32_t)RCC_CR_HSION);
	//	RCC->CR |= RCC_CR_HSION; 
	RCC->CR |= 0x00000001U;
	
  	// wait until HSI is ready
  	//while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	while ( (RCC->CR & 0x00000002U) == 0 ) {;}
	
  	// Select HSI as system clock source 
  	RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 			// not essential
  	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI; 		//00: HSI16 oscillator used as system clock

	// Wait till HSI is used as system clock source
  	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0 );

		EC_SYSCLK=16000000;
}
```



### External Clock (HSE) for GPIO

1. **Enable HSI and choose as SYSCLK source**
   - Enable HSE: **(RCC->CR: HSEON=1)** 
   - Wait until HSE is stable and ready: **(RCC->CR: HSERDY? 1)** 
   - Choose the system clock switch : **(RCC->CFGR: SW = 01)** 
   - Check if the selected source is correct: **(RCC->CFGR: SWS ? 01)**

2. **Enable GPIOx clock(AHB1ENR )** 
   - Enable (RCC_AHB1ENR) for PORTx

```c
void RCC_HSE_init(){
	
	// HSE on
	RCC->CR |= RCC_CR_HSEON;
	
	// wait until HSE is ready
	while ( (RCC->CR & RCC_CR_HSERDY) == 0 ) {;}
		
	// Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 								// not essential
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE; 								
		
		// Wait till HSE is used as system clock source
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 1 );

	EC_SYSCLK=8000000;
	
}
```



### PLL(HSI) for GPIO

1. **Enable either (HSI, or HSE) for PLL and Choose PLL for System Clock**
   - Enable HSI: **(RCC->CR : HSION=1)**
   - Wait until HSI is stable: **(RCC->CR : HSIRDY? 1)** 
   - Choose PLL for system clock switch : **(RCC->CFGR : SW = 10)**
   - Check if PLL selection is correct: **(RCC->CFGR : SWS ? 10)**
2. **Select the clock source for PLL**
   - Select the PLL source(HSI or HSE): **(RCC->PLLCFGR : PLLSRC= 0 or 1)**
3. (Optional)Configure PLL parameters
   - Select (M/N/P): **(RCC->PLLCFGR : PLLM, PLLN, …)** make 84MHz from 16MHz

4. **Enable PLL**
   - Enable main PLL: **(RCC->CR : PLLON=1), (RCC->CR : PLLRDY?0)**
5. (Optional)Configure APB/AHB Prescaler 
   - Change Prescaler: (RCC->CFGR : HPRE, PPRE)
6. **Enable GPIOx clock(AHB1ENR )**
   - Enable (RCC_AHB1ENR) for PORTx

```c
void RCC_PLL_init() {	
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  // (HCLK) and the supply voltage of the device.		
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	// Enable the Internal High Speed oscillator (HSI)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
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
```



### PLL(HSI) for GPIO

1. **Enable either (HSI, or HSE) for PLL and Choose PLL for System Clock**
   - Enable HSE: **(RCC->CR : HSEON=1)**
   - Wait until HSE is stable: **(RCC->CR : HSERDY? 1)** 
   - Choose PLL for system clock switch : **(RCC->CFGR : SW = 10)**
   - Check if PLL selection is correct: **(RCC->CFGR : SWS ? 10)**
2. **Select the clock source for PLL**
   - Select the PLL source(HSE): **(RCC->PLLCFGR : PLLSRC= 0 or 1)**
3. (Optional)Configure PLL parameters
   - Select (M/N/P): **(RCC->PLLCFGR : PLLM, PLLN, …)** make 84MHz from 8MHz

4. **Enable PLL**
   - Enable main PLL: **(RCC->CR : PLLON=1), (RCC->CR : PLLRDY?0)**
5. (Optional)Configure APB/AHB Prescaler 
   - Change Prescaler: (RCC->CFGR : HPRE, PPRE)
6. **Enable GPIOx clock(AHB1ENR )**
   - Enable (RCC_AHB1ENR) for PORTx

```c
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
```



## **GPIO digital I/O**

### GPIOx Register Table

#### GPIOx_MODER

![image](https://user-images.githubusercontent.com/91474647/196499965-43e886f9-64a4-4175-8ee6-5d18f3df6195.png)



#### GPIOx_OTYPER

![image](https://user-images.githubusercontent.com/91474647/196500209-ab1733d3-dfa9-468b-99ac-8eac70397a2a.png)



#### GPIOx_OSPEEDR

![image](https://user-images.githubusercontent.com/91474647/196500298-c017c7d5-e1de-4459-842d-33c734d8241f.png)



#### GPIOx_PUPDR

![image](https://user-images.githubusercontent.com/91474647/196500517-8d0220d2-3f4f-4504-8407-9d8cc57c0cb6.png)



#### GPIOx_IDR

![image](https://user-images.githubusercontent.com/91474647/196500686-62f5890d-0541-4199-9456-eb506535db7f.png)



#### GPIOx_ODR

![image](https://user-images.githubusercontent.com/91474647/196500799-788abb51-4eef-4055-802f-7804377891f6.png)



#### GPIOx_AFRL and GPIOx_AFRH 

using at PWM

![image](https://user-images.githubusercontent.com/91474647/196501073-06b5e0c0-33a5-4544-be37-624195f17f3f.png)

![image](https://user-images.githubusercontent.com/91474647/196501271-ebbfc649-7198-4cb4-8f34-62c0dcbf9acd.png)



### Process of GPIOx register initialization 

- **output setting** 
  - Enable Peripheral Clock (**AHB1ENR**)
  - Configure as Digital Output (**MODER**)
  - Configure pull-up/down resistors (**PUPDR**)
  - For Output: Configure Output Type (**OTYPE**)
  - For Output: Configure Output Speed (**OSPEEDR**)
- input setting
  - Enable Peripheral Clock (**AHB1ENR**)
  - Configure as Digital Output (**MODER**)

  - Configure pull-up/down resistors (**PUPDR**)

  - Input Data **(IDR)**

### ecGPIO.c

#### void GPIO_init(GPIO_TypeDef *Port, int pin, int mode)

This function is initialize of GPIO. It has three parameters, GPIO Port, pin number and mode. After choose the port, it goes enable of each port. End of the **GPIO_init** is **GPIO_mode**. 

```c
void GPIO_init(GPIO_TypeDef *Port, int pin, int mode){      
	if (Port == GPIOA)
		RCC_GPIOA_enable();
	
	if (Port == GPIOB)
		RCC_GPIOB_enable();
	
	if (Port == GPIOC)
		RCC_GPIOC_enable();
	
	if (Port == GPIOD)
		RCC_GPIOA_enable();
	
	if (Port == GPIOE)
		RCC_GPIOB_enable();
	
	GPIO_mode(Port, pin, mode);	
}
```



#### void GPIO_mode(GPIO_TypeDef *Port, int pin, int mode)

**GPIO** has 4 state of mode. **Port -> MODER** consist 2-bits. 

Input and Output are Digital signal, AlterFunc use at PWM signal and Analog

```c
// Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(GPIO_TypeDef *Port, int pin, int mode){
   Port->MODER &= ~(3UL<<(2*pin));     
   Port->MODER |= mode <<(2*pin);    
}
```



#### void GPIO_ospeed(GPIO_TypeDef *Port, int pin, int speed) 

We can select the 4 types of speed at **GPIO Speed** . **GPIO_ospeed** consist 2-bits. 

```c
// Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(GPIO_TypeDef *Port, int pin, int speed){
	Port->OSPEEDR &= ~(3UL 	<< (pin * 2));
	Port->OSPEEDR |=  speed << (pin * 2);	
}
```

#### void GPIO_otype(GPIO_TypeDef *Port, int pin, int type)

Output push-pull is using internal power source and Output open drain is using  external power source.

```c
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(GPIO_TypeDef *Port, int pin, int type){	
		Port -> OTYPER &=	~(1UL << pin);
		Port ->	OTYPER |=	(type << pin);
}
```



#### void GPIO_pupd(GPIO_TypeDef *Port, int pin, int pupd)

Pull up - pull down is prevent the "**floating**"

pull up circuit은 스위치를 누르지 않으면 5V로 인식 **HIGH**, 누르면 Input에서 **LOW**로 인식한다. 

pull down circuit은 스위치를 누르지 않으면 GND인식 **LOW**, 누르면 **HIGH**로 인식

```c
// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(GPIO_TypeDef *Port, int pin, int pupd){
		Port->PUPDR  &= 	~(3UL << (pin * 2));			
		Port->PUPDR  |= 	(pupd	<< (pin * 2));			
}
```



#### int GPIO_read(GPIO_TypeDef *Port, int pin)

```c
int GPIO_read(GPIO_TypeDef *Port, int pin){ 
	// 0 or 1만 읽기 위해서 사용하는 방법
	return ((Port -> IDR) >> pin) & 1UL;	
}
```



#### void GPIO_write(GPIO_TypeDef *Port, int pin,  int output)

```c
void GPIO_write(GPIO_TypeDef *Port, int pin,  int output){
		 if(output == 1)	
				Port->ODR |= (1UL << pin);	 	
			else
				Port->ODR &= ~(1UL << pin); 
}
```



#### void GPIO_in_set(GPIO_TypeDef *Port, int pin, int pupd)

**GPIO** input setting

set port, pin number, pull up pull down

```c
void GPIO_in_set(GPIO_TypeDef *Port, int pin, int pupd){	
	GPIO_init(Port, pin, INPUT);
	GPIO_pupd(Port, pin, pupd);
}
```



#### void GPIO_out_set(GPIO_TypeDef *Port, int pin, int pupd, int speed, int type)

**GPIO** output setting

set port, pin number, pull up pull down, speed, pushpull

```c
void GPIO_out_set(GPIO_TypeDef *Port, int pin, int pupd, int speed, int type){	
	GPIO_init		(Port, pin, OUTPUT);			
	GPIO_otype		(Port, pin, type);			
	GPIO_pupd		(Port, pin, pupd);				
	GPIO_ospeed		(Port, pin, speed);			
	GPIO_write		(Port, pin, LOW); 			
}
```



## **External Interrupt**

### EXTI Register Table

#### EXTI_IMR

![image](https://user-images.githubusercontent.com/91474647/196504861-61dce022-390c-4b8b-a5ff-5c6393ca78f2.png)



#### EXTI_EMR

![image](https://user-images.githubusercontent.com/91474647/196505181-136f8f3b-5d36-4602-b483-35c1b991bb45.png)



#### EXTI_RTSR

![image](https://user-images.githubusercontent.com/91474647/196505378-7cfd0d39-a9a6-447b-a652-d4b7f47fbbf6.png)



#### EXTI_FTSR

![image](https://user-images.githubusercontent.com/91474647/196505462-32a2621b-5683-4c13-a4c9-5e328de906fb.png)



#### EXTI_PR

![image](https://user-images.githubusercontent.com/91474647/196505698-f6b0ca7b-1d16-4199-9260-ff3ba4f5e3a5.png)



### ecEXTI.c

#### void EXTI_init(GPIO_TypeDef *Port, int Pin, int trig_type, int priority)

1. Go to SYSCFG peripheral and clock enable	
2. Select the port
3. Select the pin at **EXTI_CR**
4. Choose the Falling or Rising at **FTSR** or **RTSR**
5. Interrupt enable **IMR**
6. Array address allocation at EXTI_IRQn 0~4, 5~9 and 10 ~ 15
7. Priority of pending allocation 
8. EXTI enable 

```c
void EXTI_init(GPIO_TypeDef *Port, int Pin, int trig_type, int priority){  

	// SYSCFG peripheral clock enable	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect External Line to the GPIO
	unsigned int EXTICR_port; 
	// 미리 지정되어 있는 포트의 일반화 번호들이다
	if		(Port == GPIOA) EXTICR_port = 0;
	else if	(Port == GPIOB) EXTICR_port = 1;
	else if	(Port == GPIOC) EXTICR_port = 2;
	else if	(Port == GPIOD) EXTICR_port = 3;
	else 					EXTICR_port = 4;
	
	/*	SYSCFG->EXTICR[BUTTON_PIN/4] &= ~SYSCFG_EXTICR4_EXTI13; //~15<<(4* (pin%4))*/
	SYSCFG->EXTICR[Pin/4] &= ~( 15UL << (4*(Pin%4)) ) ;			// clear 4 bits
	SYSCFG->EXTICR[Pin/4] |= (EXTICR_port << (4*(Pin%4)) ) ;	// set 4 bits  무슨 포트인지 일반화
	
	// Configure Trigger edge
	if 		(trig_type == FALL) EXTI->FTSR |= (1UL << Pin);   // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= (1UL << Pin);   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable 
		EXTI->RTSR |= (1UL << Pin); 
		EXTI->FTSR |= (1UL << Pin);
	} 
	
	// Configure Interrupt Mask (Interrupt enabled)
	EXTI->IMR  |= (1UL << Pin);     // not masked, 왜 not masked 였지?
		
	// NVIC(IRQ) Setting
	uint8_t EXTI_IRQn = 0; 
	
	if (Pin < 5) 	EXTI_IRQn = Pin + 6;  					// EXTI0이 6번 핀을 가지고 있다. EXTI4 는 10번 핀에 할당되어 있다.
	else if	(Pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn; // 5~9번핀일 때  EXTI9_5_IRQn -> 23번핀에 할당
	else 			EXTI_IRQn = EXTI15_10_IRQn;  				// 10 ~ 15번 핀  40번 할당
								
	NVIC_SetPriority(EXTI_IRQn, priority);	// NVIC sets the order of execution according to prior
	NVIC_EnableIRQ(EXTI_IRQn); 	// EXTI IRQ enable
}
```



#### void EXTI_enable(uint32_t pin)

```c
void EXTI_enable(uint32_t pin) {
	EXTI->IMR |= (1UL << pin);     // not masked (i.e., Interrupt enabled)
}
```



#### void EXTI_disable(uint32_t pin)

```c
void EXTI_disable(uint32_t pin) {
	EXTI->IMR &= ~(1UL << pin);     // masked (i.e., Interrupt disabled)
}
```



#### uint32_t is_pending_EXTI(uint32_t pin)

This function is return 1 or 0. EXTI로 지정한 pin 에서 signal이 들어오면 pending structure에서 비교를 하여 if 문 안의 조건을 실행할지 말지 결정한다

```c
uint32_t is_pending_EXTI(uint32_t pin){
	uint32_t EXTI_PRx = 1UL << pin;     	// check  EXTI pending 	
	return ((EXTI->PR & EXTI_PRx) == EXTI_PRx);
}
```



#### void clear_pending_EXTI(uint32_t pin)

You have to use this code after the Interrupt was began. If it does not use will gives huge error.

```c
void clear_pending_EXTI(uint32_t pin){
	EXTI->PR |= 1UL << pin;     // clear EXTI pending 
}
```



### ecSysTick.c

systick is also interrupt, but it is down count system. And Systick's pending order is subordinate.

```c
void SysTick_init(uint32_t msec){	

	//  SysTick Control and Status Register
	// Disable SysTick IRQ and SysTick Counter
	SysTick_disable();								

	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// SysTick Reload Value Register
	SysTick->LOAD = (MCU_CLK_PLL / (1000)) * msec - 1;						// 1ms, for HSI PLL = 84MHz.
	
	// SysTick Current Value Register
	SysTick_reset();

	// Enables SysTick exception request " 이거 이해가 잘 안간다"
	// 0 = Counting down to zero does not assert the SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
		
	// Enable SysTick IRQ and SysTick Timer
	SysTick_enable();
		
	NVIC_SetPriority(SysTick_IRQn, 16);		// Set Priority to 16
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC
}
```



**Down count delay set**

```c
void SysTick_Handler(void){
	SysTick_counter();	
}

void SysTick_counter(void){
	TimeDelay--;
}	

void Delay (uint32_t nTime){
	
	TimeDelay = nTime; //setup 
	while(TimeDelay != 0);	
}
```



**void SysTick_reset(void)** 

set the **VAL** value goes to 0

```c
	// SysTick -> VAL에서 VAL이 0이 되면 feedback loop에서 reload 값으로 다시 돌아간다
	// SysTick을 초기화시키는 방식 -> 다운 클락이기 때문에 가능한 방법이고 
void SysTick_reset(void)  {
	SysTick->VAL = 0;			// if VAL is 0, VAL will update Reroad value
}
```



**uint32_t SysTick_val(void)**

check the current **VAL** value

```
uint32_t SysTick_val(void) {	
	return SysTick->VAL;
}
```



**void SysTick_enable(void)**

```c
void SysTick_enable(void){		
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;		
}
```



**void SysTick_disable(void)**

```c
void SysTick_disable(void){
	SysTick->CTRL = 0;	
}
```



### ecFunc.c

#### void sevensegment_init(void)

**GPIO** output setting for 7segment initialization 

```c
void sevensegment_init(void) {
	// led a setup
	GPIO_out_set(GPIOA, PA8, NOPUPD, MSPEED, PUSHPULL);
	// led b setup
	GPIO_out_set(GPIOB, PB10, NOPUPD, MSPEED, PUSHPULL);
  // led c setup
	GPIO_out_set(GPIOA, PA7, NOPUPD, MSPEED, PUSHPULL);									
	// led d setup
	GPIO_out_set(GPIOA, PA6, NOPUPD, MSPEED, PUSHPULL);
	// led e setup
	GPIO_out_set(GPIOA, PA5, NOPUPD, MSPEED, PUSHPULL);
	// led f setup
	GPIO_out_set(GPIOA, PA9, NOPUPD, MSPEED, PUSHPULL);
	// led g setup
	GPIO_out_set(GPIOC, PC7, NOPUPD, MSPEED, PUSHPULL);
	// led DP setup
	GPIO_out_set(GPIOB, PB6, NOPUPD, MSPEED, PUSHPULL);		
}
```



#### void sevensegment_decoder(uint8_t flag)

seven_segment is second array. The array gives flag to GPIO_write.

```c
void sevensegment_decoder(uint8_t flag) {
	int seven_segment[11][8] = {
			{LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH},        //zero
			{HIGH,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH},        //one
			{LOW,	LOW,	HIGH,	LOW,	LOW,	HIGH,	LOW,	HIGH},        //two
			{LOW,	LOW,	LOW,	LOW,	HIGH,	HIGH,	LOW,	HIGH},        //three
			{HIGH,	LOW,	LOW,	HIGH,	HIGH,	LOW,	LOW,	HIGH},        //four
			{LOW,	HIGH,	LOW,	LOW,	HIGH,	LOW,	LOW,	HIGH},        //five
			{LOW,	HIGH,	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH},        //six
			{LOW,	LOW,	LOW,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH},        //seven
			{LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	LOW,	HIGH},        //eight
			{LOW,	LOW,	LOW,	HIGH,	HIGH,	LOW,	LOW,	HIGH},        //nine
			{HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW}          //dot
	};
    
	GPIO_write(GPIOA, PA8, seven_segment[flag][0]); 		// a
	GPIO_write(GPIOB, PB10, seven_segment[flag][1]); 		// b
	GPIO_write(GPIOA, PA7, seven_segment[flag][2]); 		// c
	GPIO_write(GPIOA, PA6, seven_segment[flag][3]); 		// d
	GPIO_write(GPIOA, PA5, seven_segment[flag][4]); 		// e
	GPIO_write(GPIOA, PA9, seven_segment[flag][5]); 		// f
	GPIO_write(GPIOC, PC7, seven_segment[flag][6]); 		// g
	GPIO_write(GPIOB, PB6, seven_segment[flag][7]); 		// dp
}
```



#### void bitToggle(GPIO_TypeDef* Port, int pin)

Bit toggle use XOR 

```c
void bitToggle(GPIO_TypeDef* Port, int pin) {
	Port->ODR ^= (1 << pin);
}
```



#### void LED4_toggle(uint8_t flag)

```c
void LED4_toggle(uint8_t flag){ 	// use truth table of 4 leds circuit	
	int led4[4][4] = { 
		{HIGH,		LOW,		LOW,		LOW},
		{LOW,		HIGH,		LOW,		LOW},
		{LOW,		LOW,		HIGH,		LOW},
		{LOW,		LOW,		LOW,  		HIGH}
	};
		GPIO_write(GPIOA, PA5, led4[flag][0]); 		// PA5
		GPIO_write(GPIOA, PA6, led4[flag][1]); 		// PA6
		GPIO_write(GPIOA, PA7, led4[flag][2]); 		// PA7
		GPIO_write(GPIOB, PB6, led4[flag][3]); 		// PB6d
}
```















