/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
@ Embedded Controller edit by Seung-Eun Hwang 
Created          : 05-03-2021
Modified         : 10-14-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/



//#include "stm32f4xx.h"
//#include "stm32f411xe.h"
//#include "ecGPIO.h"

#include "ecInclude.h"

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


// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(GPIO_TypeDef *Port, int pin, int mode){
   Port->MODER &= ~(3UL<<(2*pin));     
   Port->MODER |= mode <<(2*pin);    
}


// GPIO Speed          : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(GPIO_TypeDef *Port, int pin, int speed) {
	
		Port->OSPEEDR &= ~(3UL 	<< (pin * 2));
		Port->OSPEEDR |=  speed << (pin * 2);	
}

// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(GPIO_TypeDef *Port, int pin, int type){
	
		Port -> OTYPER &=	~(1UL << pin); // Port -> OTYPER &= ~(type << pin); 이거 좀 이상하다. 원래 이렇게 적혀있었다.
		Port ->	OTYPER |=	(type << pin);
}

// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(GPIO_TypeDef *Port, int pin, int pupd){
		Port->PUPDR  &= 	~(3UL << (pin * 2));			
		Port->PUPDR  |= 	(pupd	<< (pin * 2));			
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
			
			
			// 	Port->ODR &= ~(1UL << pin);		// clear
			//	Port->ODR |= (output << pin);	// set
	
}

// GPIO input setting
// set port, pin number, pull up pull down
void GPIO_in_set(GPIO_TypeDef *Port, int pin, int pupd){	
	GPIO_init(Port, pin, INPUT);
	GPIO_pupd(Port, pin, pupd);
}
	

// GPIO output setting
// set port, pin number, pull up pull down, speed, pushpull
void GPIO_out_set(GPIO_TypeDef *Port, int pin, int pupd, int speed, int type){	
	
	GPIO_init		(Port, pin, OUTPUT);			
	GPIO_otype		(Port, pin, type);			
	GPIO_pupd		(Port, pin, pupd);				
	GPIO_ospeed		(Port, pin, speed);			
	GPIO_write		(Port, pin, LOW); 			
}


void GPIO_AF_set(GPIO_TypeDef *Port, int pin, int pupd, int speed, int type){
	
	
	GPIO_init		(Port, pin, AF);			
	
	if(pupd != DEFAULT){
		GPIO_pupd		(Port, pin, pupd);	
	}
	if(speed != DEFAULT){
		GPIO_ospeed		(Port, pin, speed);			
	}
	if(type != DEFAULT){		
		GPIO_otype		(Port, pin, type);		
	}		
	GPIO_write		(Port, pin, LOW); 	
}
	 




