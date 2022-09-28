/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
@ Embedded Controller edit by Seung-Eun Hwang 
Author           : SSS LAB
Created          : 05-03-2021
Modified         : 09-27-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/



#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecGPIO.h"


void GPIO_init(GPIO_TypeDef *Port, int pin, int mode){     
	// mode  : Input(0), Output(1), AlterFunc(2), Analog(3)   
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


// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(GPIO_TypeDef *Port, int pin, int mode){
   Port->MODER &= ~(3UL<<(2*pin));     
   Port->MODER |= mode <<(2*pin);    
}


// GPIO Speed          : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(GPIO_TypeDef *Port, int pin, int speed){
	
		Port->OSPEEDR &= ~(3UL 	<< (pin * 2));
		Port->OSPEEDR |=  speed << (pin * 2);	//10:Fast Speed
	
}

// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(GPIO_TypeDef *Port, int pin, int type){
	
		Port -> OTYPER &= ~(type << pin);		// 0:Push-Pull  
	
}

// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(GPIO_TypeDef *Port, int pin, int pupd){
	
		Port->PUPDR  &= 	~(3UL << (pin * 2));		// 00: claer
		Port->PUPDR  |= 	(pupd	<< (pin * 2));			// 00: write
	
}



int GPIO_read(GPIO_TypeDef *Port, int pin){
  
	// 0 or 1만 읽기 위해서 사용하는 방법
	return ((Port -> IDR) >> pin) & 1UL;
	
	
}

void GPIO_write(GPIO_TypeDef *Port, int pin,  int output){
	
		 if(output == 1)
				
				Port->ODR |= (1UL << pin);	 	
			
			else
				
				Port->ODR &= ~(1UL << pin); 
	
}


void bitToggle ( GPIO_TypeDef *Port, int pin){
	
	Port -> ODR ^= (1 << pin);
	
}
