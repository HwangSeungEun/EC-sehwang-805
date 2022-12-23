# Final Project

**Date:** 2022-12-23

**Author/Partner:** SeungEung Hwang / Hanse Cho (21800805 / 21800703 )

**Github:** [repository link](https://github.com/HwangSeungEun/EC-sehwang-805)

**Demo Video:** [demo video](https://youtu.be/Tj8IBTjQ2kM)



## I. Introduction

In this Final project, it produced a miniaturized automatic delivery system. The reason for choosing the automatic delivery system is that it fits well with the current direction of industrial science and technology development and is in the spotlight in the modern industry. Therefore, I thought it was an industry that could not be left out in modern society. As evidenced by this, IT companies such as Amazon and Google are investing heavily for delivery automation systems and are striving to expand their infrastructure. We designed using several stm32 and Hal libray and several sensors to design an automated delivery system.




![IMG_3626](https://user-images.githubusercontent.com/91474647/209276964-503aacea-2cb7-4b04-8a4b-be729611f42d.JPG)



![IMG_3625](https://user-images.githubusercontent.com/91474647/209277153-af7f12c1-7484-4065-a76f-3eab3c3a1762.JPG)





### Requirement

#### Hardware

- **MCU**
  - NUCLEO-F411RE x3

- **Actuator**
  - DC Motor Driver (LS9110s)
  - DC Motor (SZH-GNP193) x2 
  - LED x3 
  - Servo motor 360 (FS5106R) x3
  - RC Servo Motor (SG90)
  - Conveyor belt
- **Sensor**
  - IR Reflective Sensor (TCRT 5000) x2
  - Ultra Sonic Sensor (HC-SR04)
  - RGB color sensor (TCS3200) x4
  - Piezoelectric sensor (FSR 406)
  - CDS illumination sensor (SZH-SSBH-011)
- **Communication equipment**
  - Ziggbe
- **Others**
  - Spring in Factory
  - Battery(5V)

#### Software

- Keil uVision
- CMSIS
- EC_HAL library



## II. Hardware Description

### 1. Servo motor 360 (FS5106R)

![image](https://user-images.githubusercontent.com/113971258/209290964-eccd02d2-844e-4414-b2b6-230e218fe7cf.png)

- PWM Period: 20msec

- Pulse width range: 700～2300µsec
- Stop position (Stop rotate): 1500(±5) µsec
- Rotating direction: 
  - CW: 700~1500 µsec
  - CCW: 1500~2300 µsec

- Our Servo motor 360 setting: 0.6 - duty_ratio(12ms)

### 2. RGB color sensor (TCS3200)

<img src="https://user-images.githubusercontent.com/113971258/209294375-21382319-aa73-4a71-a417-264020edab52.png" alt="image" style="zoom: 33%;" /><img src="https://user-images.githubusercontent.com/113971258/209294679-f911354f-a108-47eb-8790-bc0dc23d8e49.png" alt="image" style="zoom: 80%;" />

![image](https://user-images.githubusercontent.com/113971258/209294503-77c2fc2a-6206-4c9c-a8be-e09ea90b9007.png)

![image](https://user-images.githubusercontent.com/113971258/209294579-3b0ae32a-b23e-4006-b725-84ada939acac.png)

- #### Manual

  - Set the scale at 2% (S0=L,S1=H): The input capture value is different depending on the scale. The smaller the scale, the larger the resolution.
  - Photodiode type: If you want sensing red color, setting S2=L, S3=L.
  - Sensing particular color: When a particular color is strongly sensed, output square wave frequency increases. That is, the input capture value becomes small.
  - Color determination: 
    - Frequency offset setting: Since R, G, and B have different offset frequencies when detecting white color, they must be corrected first.
    - Sensing red, green, and blue colors once every one second. The color of the smallest value among the sensed input capture values is the detected color. This is because if the input capture value is small, the Ouput-frequency is large.

### 3. Piezoelectric sensor (FSR 406)

<img src="https://user-images.githubusercontent.com/113971258/209301123-efcc0d04-5246-41c3-99b6-a434909ab8ab.png" alt="image" style="zoom: 67%;" /><img src="https://user-images.githubusercontent.com/113971258/209300838-21f3c364-67b9-4dca-90df-2b093a5684ac.png" alt="image"  />

- This sensor is a variable resistance according to pressure.
- Technically, this sensor is an analog sensor, but it was viewed as a digital sensor, because we only care about on/off of sensor value. 
- We use following Algorithm
  - Pressure Sensor Value = GPIO_read(Port, Pin)



### 4. Illumination Sensor(SZH-SSBH-011)

![image](https://user-images.githubusercontent.com/91474647/209310680-8577b8d7-c737-46a8-836e-22ab28ddc2d1.png)

- Detecting light intensity, light intensity sensors, smart car hunt optical module
- Using sensitive photoresistor type sensor
- The comparator output signal clean, good waveform, driving ability over 15mA
- The operating voltage: 5V 3.3V - Output type: DO digital switching output (0 and 1) and AO analog voltage output
- We get the value int algorithm 
  - ADC Injection 



### 5. Spring in Factory

|            Example Image (spring vending machine)            |
| :----------------------------------------------------------: |
| ![image](https://user-images.githubusercontent.com/113971258/209305846-2bc7b562-5e02-48c0-b381-45c67cd41010.png) |

- In factory(MCU1), We ship goods from a factory with a spring combined with a servo motor



## III. Problem Description

This system automatically delivers when you enter the product and delivery destination information on the headquarters server. A simple schematic diagram is shown below.

![image](https://user-images.githubusercontent.com/91474647/208845312-a6071a27-9343-45a1-864b-380b141c30e7.png)															

### Configuration 

- #### **Factory (MCU1)**

![image](https://user-images.githubusercontent.com/91474647/209304171-2c722610-7e15-44b3-bfff-3079155d8a39.png)



- #### **Delivery Spot(MCU2)**

![image](https://user-images.githubusercontent.com/91474647/209304096-adab7095-f8f9-4fd0-8e17-1a8288b299a6.png)



- #### **Car(MCU3)**

![image](https://user-images.githubusercontent.com/91474647/209304025-716e4f25-25d0-4804-af83-571be814e00d.png)



### Circuit Diagram

- #### Factory (MCU1)

![image](https://user-images.githubusercontent.com/91474647/209306839-761d90d5-aa59-45a9-a063-ef70b4bf0c22.png)

- #### Delivery Spot(MCU2)

![image](https://user-images.githubusercontent.com/91474647/209307012-aeaf6bc4-5a68-4e48-9ad3-4fd3e73321f4.png)

- #### Car(MCU3)

![image](https://user-images.githubusercontent.com/91474647/209307075-29ca03f8-f18a-4b2c-a868-ef63d8380ce0.png)



## IV. Algorithm

- #### flow chart

![image](https://user-images.githubusercontent.com/91474647/207004845-c72a9dce-1b0d-43ff-b047-3008d1e2ee37.png)

- #### Scenario Table

![image](https://user-images.githubusercontent.com/91474647/208896396-bff8927b-fa98-4673-8396-1e60b286bec0.png)



## V. Demonstration

[demo video](https://youtu.be/Tj8IBTjQ2kM)  

<iframe width="956" height="538" src= "https://www.youtube.com/embed/Tj8IBTjQ2kM" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>



## VI. Appendix

### Code

- #### Factory (MCU1)

```c
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Hanse Cho
Created          : 12-07-2022
Modified         : 12-18-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for EC_Final_Project MCU1
/----------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecSysTick.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecRGBCS.h"
#include "ecMCU1.h"

#define WAIT_DELV_FLAG 0
#define WAIT_PRDCT_FLAG 1
#define START_FLAG 2
uint8_t flag = 0;

#define END_CHAR 13
#define MAX_BUF 100

#define WAIT_FINISH_DELV_STATE 4

uint32_t zgbData = 0;
#define Rst_zgbData 0
uint32_t pcData = 0;

int Product = 0;
int Delivery = 0;
#define Rst_Delv 0
#define Rst_Prdct 0

uint32_t rgb1_ovf_cnt = 0;
uint32_t rgb1_rise = 0;
uint32_t rgb1_fall = 0;
uint32_t rgb1_val = 0;

#define LED_PIN 	5

PWM_t PWM1; // product = R
PWM_t PWM2; // product = G
PWM_t PWM3; // product = B

PWM_t PWM4;

void setup(void);
float servodeg2pwmratio(float servodeg);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	uint32_t state = 0;	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		if		 (Delivery == Rst_Delv){flag = WAIT_DELV_FLAG;}
		else if(Product == Rst_Prdct){flag = WAIT_PRDCT_FLAG;}
		else												 {flag = START_FLAG;}
		
		switch(flag){
			case WAIT_DELV_FLAG:
				// wait for Delivey input
				printf("\033[H\033[2J"); 
				printf("Input_Delivery Adress(1,2,3): "); 
				delay_ms(2000); 
				continue;
			
			case WAIT_PRDCT_FLAG:
				// wait for Product input
				printf("\33[2K\r");
				printf("Input_Product(R,G,B): "); 
				delay_ms(2000); 
				continue;
			
			case START_FLAG:
				// Receive the delivery address and Product
				if(Product == Rst_Prdct){state = 0;}
				else if(Product == 'R') {state = 1; RGBCS_config(RGB_R); }
				else if(Product == 'G') {state = 2; RGBCS_config(RGB_G); }
				else if(Product == 'B') {state = 3; RGBCS_config(RGB_B); }
				MCU1_Out(state);
				MCU1_Out(WAIT_FINISH_DELV_STATE);
				break;
		}
		
		// wait for finish delivery
		while(!(zgbData == 'S' || zgbData == 'F')){}
			
		// start when finish delivery
		if		 (zgbData == 'S'){printf("Success: ");}
		else if(zgbData == 'F'){printf("Fail: ");}
		printf("Delivery Address = %c, Product = %c\r\n",Delivery, Product);
		PWM_duty(&PWM4,servodeg2pwmratio(89));
		
		Delivery = Rst_Delv;
		Product = Rst_Prdct;
		zgbData = Rst_zgbData;
		delay_ms(10000);
	}
}
// Initialiization  
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_init(1);
	USART_init(USART2, 9600);
	USART_begin(USART6, GPIOA, 11, GPIOA, 12, 9600);
	
	MCU1_init(&PWM1,GPIOB, 6,&PWM2, GPIOB, 7,&PWM3, GPIOB, 9);	
	
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	RGBCS_init();
	RGBCS_freqScale(RGB_20_PER);
	GPIO_init_out(GPIOC,10, OUTPUT,1,0,EC_PU);
	GPIO_write(GPIOC,10,1);
	
	GPIO_init_out(GPIOA,5, OUTPUT,1,0,EC_PU);
	PWM_init_ms(&PWM4, GPIOA, 6,20);
	PWM_duty(&PWM4,servodeg2pwmratio(90));
	
	IC_t RGB1;												// Input Capture for echo
	ICAP_init(&RGB1, RGB1_PORT, RGB1_PIN);    		// PB10 as input caputre
 	ICAP_counter_us(&RGB1, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&RGB1, 1, IC_RISE);   	// TIM2_CH1 as IC1 , rising edge detect
	ICAP_setup(&RGB1, 2, IC_FALL);   	// TIM2_CH1 as IC2 , falling edge detect
	
}

void USART2_IRQHandler(){         //USART2: COM-FT(Factory)(MCU1)
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART6, &pcData, 1);
		printf("%c", pcData);
		if(pcData == '1' || pcData == '2' || pcData == '3'){Delivery = pcData; printf("\033[H\033[2J"); printf("Delivery: %c\r\n", pcData);}
		else if(pcData == 'R' || pcData == 'G' || pcData == 'B'){Product = pcData;}
		if (pcData == END_CHAR) printf("\r\n");
	}
}

void USART6_IRQHandler(){
	if(is_USART_RXNE(USART6)){
		zgbData = USART_getc(USART6);
//		printf("%c", zgbData);	
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		rgb1_ovf_cnt++;													// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 1)){ 									// TIM2_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb1_rise = TIM2->CCR1;									// Capture TimeStart
		clear_CCIF(TIM2, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2, 2)){ 								// TIM2_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb1_fall = TIM2->CCR2;										// Capture TimeEnd
		rgb1_val = ((rgb1_fall-rgb1_rise) + (0xFFFF+1)*rgb1_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		
		if(Product == 'R'){if(rgb1_val < 11){PWM_duty(&PWM4,servodeg2pwmratio(0));}}
		else if(Product == 'G'){if(rgb1_val < 11){PWM_duty(&PWM4,servodeg2pwmratio(0));}}
		else if(Product == 'B'){if(rgb1_val < 11){PWM_duty(&PWM4,servodeg2pwmratio(0));}}
		
		rgb1_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 2);								  // clear capture/compare interrupt flag 
	}
}

float servodeg2pwmratio(float servodeg){ // unit conversion deg to ratio of pwm
	return (0.1/180.0)*servodeg + 0.025; // 0.5ms~2.5ms => 0.025 ~ 0.125 ratio
}
```





- #### Delivery Spot(MCU2)

```c
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Hanse Cho LAB
Created          : 12-07-2022
Modified         : 12-18-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for EC_Final_Project MCU2
/----------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecSysTick.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecRGBCS.h"
#include "ecMCU1.h"

#define END_CHAR 13
#define MAX_BUF 100

#define DELIV1_LASER_PORT GPIOB
#define DELIV2_LASER_PORT GPIOC
#define DELIV3_LASER_PORT GPIOC
#define DELIV1_LASER_PIN 0
#define DELIV2_LASER_PIN 1
#define DELIV3_LASER_PIN 0

PWM_t PWM5;

uint32_t zgbData = 0;
uint32_t pcData = 0;
uint32_t buffer[MAX_BUF] = {0, };
int bReceive = 0;
int idx = 0;

int Product = 0;
int Delivery = 0;
uint8_t Feedback = 0;

uint32_t rgb1_ovf_cnt = 0;
uint32_t rgb1_rise = 0;
uint32_t rgb1_fall = 0;
uint32_t rgb1_val = 0;

uint32_t rgb2_ovf_cnt = 0;
uint32_t rgb2_rise = 0;
uint32_t rgb2_fall = 0;
uint32_t rgb2_val = 0;

uint32_t rgb3_ovf_cnt = 0;
uint32_t rgb3_rise = 0;
uint32_t rgb3_fall = 0;
uint32_t rgb3_val = 0;

uint32_t rgb_val = 0;
uint32_t rgb_val_min = 0;

uint32_t Red_val = 0;
uint32_t Green_val = 0;
uint32_t Blue_val = 0;
uint32_t RGB_cnt = 0;
uint32_t RGB_cnt_min = 0;
uint32_t Detect_val = 0;

uint32_t Detect_cnt = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		if(Product != 0 && Delivery != 0){
			// setting rgb sensor config
			RGBCS_config(RGB_cnt);
			if		 (Delivery == '1'){rgb_val = rgb1_val; GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,1); GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,0); GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,0);}
			else if(Delivery == '2'){rgb_val = rgb2_val; GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,0); GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,1); GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,0);}
			else if(Delivery == '3'){rgb_val = rgb3_val; GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,0); GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,0); GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,1);}
			if(300<rgb_val){
				//// Q. why RGB_cnt = 1 -> rgb vale = R? 
				//// A. we get rgb_val before start this loop, and when start loop we set RGBCS_config, this setting is applied next loop.
				//// RGBCS_config(RGB_cnt);
				delay_ms(1000);
				
			// rgb offset set		
				// delivery = 1
				if(Delivery == '1'){
					if(RGB_cnt == 0){rgb_val += 100;}
					else if(RGB_cnt == 1){rgb_val -= 0;}
					else if(RGB_cnt == 2){rgb_val += 50;}
				}
				// delivery = 2
				if(Delivery == '2'){
					if(RGB_cnt == 0){rgb_val += 90;}
					else if(RGB_cnt == 1){rgb_val -= 10;}
					else if(RGB_cnt == 2){rgb_val -= 50;}
				}
				// delivery = 3
				if(Delivery == '3'){
					if(RGB_cnt == 0){rgb_val += 100;}
					else if(RGB_cnt == 1){rgb_val -= 0;}
					else if(RGB_cnt == 2){rgb_val -= 10;}
				}
				
				// display cnt, rgb value 
				printf("cnt:%d, %d\r\n",RGB_cnt,rgb_val);
				
				// select min value of cnt, rgb value of R,G,B
				if(rgb_val > rgb_val_min){rgb_val_min = rgb_val_min; RGB_cnt_min = RGB_cnt_min;}
				else if(rgb_val <= rgb_val_min){rgb_val_min = rgb_val; RGB_cnt_min = RGB_cnt;}
				
				// 
				if(RGB_cnt == 2 && rgb_val_min != 0){
					if(Detect_cnt >= 1){
						printf("rgb_val_min: %d, %d\r\n",rgb_val_min,RGB_cnt_min);
						if(RGB_cnt_min == 0){Detect_val = 'B';}
						else if(RGB_cnt_min == 1){Detect_val = 'R';}
						else if(RGB_cnt_min == 2){Detect_val = 'G';}
						
						if(Product == Detect_val){Feedback = 'S'; USART_write(USART6, &Feedback, 1);}
						else 										 {Feedback = 'F'; USART_write(USART6, &Feedback, 1);}
					//	printf("Finish\r\n, Feedback: %c",Feedback);
						
						Product = 0;
						Delivery = 0;
						GPIO_write(DELIV1_LASER_PORT, DELIV1_LASER_PIN,0);
						GPIO_write(DELIV2_LASER_PORT, DELIV2_LASER_PIN,0);
						GPIO_write(DELIV3_LASER_PORT, DELIV3_LASER_PIN,0);
						
//						break;
					}
					Detect_cnt++;
				}
				
				// reset detect cnt
				if(Detect_cnt >= 5 ){Detect_cnt = 0;}
				RGB_cnt++;
				
				// when get R,G,B value all, reset all
				if(RGB_cnt>2){RGB_cnt = 0; rgb_val_min = 10e6; RGB_cnt_min = 10;}
			}
			
			// when no car but lane detect, print no detect(product)
			else{
				printf("No detect\r\n"); 
				delay_ms(1000);
			}		
		}	
	}			
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	USART_init(USART2, 9600);
	USART_begin(USART6, GPIOA, 11, GPIOA, 12, 9600);
	
	// Delivery laser pin
	GPIO_init_out(DELIV1_LASER_PORT,DELIV1_LASER_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(DELIV2_LASER_PORT,DELIV2_LASER_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(DELIV3_LASER_PORT,DELIV3_LASER_PIN, OUTPUT,1,0,EC_PU);
	
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	RGBCS_init();
	RGBCS_freqScale(RGB_2_PER);
	GPIO_init_out(GPIOC,10, OUTPUT,1,0,EC_PU);
	GPIO_write(GPIOC,10,1);
	PWM_init_us(&PWM5, GPIOA, 8,1000);
	PWM_duty(&PWM5,0.6);
	
	GPIO_init_out(GPIOA,5, OUTPUT,1,0,EC_PU);
	GPIO_init(GPIOC, 13, INPUT);
	GPIO_pupdr(GPIOC, 13, EC_PU);

	// Input Capture configuration -----------------------------------------------------------------------	
	RGBCS_IC_config();
	

}

void USART6_IRQHandler(){
	if(is_USART_RXNE(USART6)){
		zgbData = USART_getc(USART6);
		if		 (zgbData == 'R' || zgbData == 'G' || zgbData == 'B'){Product = zgbData;}
		else if(zgbData == '1' || zgbData == '2' || zgbData == '3'){Delivery = zgbData;}
		printf("%c", zgbData);	
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		rgb1_ovf_cnt++;													// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 1)){ 									// TIM2_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb1_rise = TIM2->CCR1;									// Capture TimeStart
		clear_CCIF(TIM2, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2, 2)){ 								// TIM2_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb1_fall = TIM2->CCR2;										// Capture TimeEnd
		rgb1_val = ((rgb1_fall-rgb1_rise) + (0xFFFF+1)*rgb1_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		rgb1_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 2);								  // clear capture/compare interrupt flag 
	}
}

void TIM3_IRQHandler(void){
	if(is_UIF(TIM3)){                     	// Update interrupt
		rgb2_ovf_cnt++;													// overflow count
		clear_UIF(TIM3);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM3, 1)){ 									// TIM3_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb2_rise = TIM3->CCR1;									// Capture TimeStart
		clear_CCIF(TIM3, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM3, 2)){ 								// TIM3_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb2_fall = TIM3->CCR2;										// Capture TimeEnd
		rgb2_val = ((rgb2_fall-rgb2_rise) + (0xFFFF+1)*rgb2_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		rgb2_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 2);								  // clear capture/compare interrupt flag 
	}
}

void TIM4_IRQHandler(void){
	if(is_UIF(TIM4)){                     	// Update interrupt
		rgb3_ovf_cnt++;													// overflow count
		clear_UIF(TIM4);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM4, 1)){ 									// TIM4_Ch1 (IC1) Capture Flag. Rising Edge Detect
		rgb3_rise = TIM4->CCR1;									// Capture TimeStart
		clear_CCIF(TIM4, 1);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM4, 2)){ 								// TIM4_Ch1 (IC2) Capture Flag. Falling Edge Detect
		rgb3_fall = TIM4->CCR2;										// Capture TimeEnd
		rgb3_val = ((rgb3_fall-rgb3_rise) + (0xFFFF+1)*rgb3_ovf_cnt);// Total time of echo pulse (10us * counter pulses -> [sec] unit)
		rgb3_ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM4, 2);								  // clear capture/compare interrupt flag 
	}
}
```



- #### ecRGBCS.c

```c
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Hanse Cho
Created          : 12-07-2022
Modified         : 12-18-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for EC_Final_Project RGB sensor
/----------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecRGBCS.h"

void RGBCS_init(void){
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	GPIO_init_out(S0_PORT,S0_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(S1_PORT,S1_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(S2_PORT,S2_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(S3_PORT,S3_PIN, OUTPUT,1,0,EC_PU);
	GPIO_init_out(CS_EN_PORT,CS_EN_PIN, OUTPUT,1,0,EC_PU);
}

void RGBCS_freqScale(int mode){
	if(mode == RGB_PWRDWN){
		// Set Power Down
	GPIO_write(S0_PORT,S0_PIN,LOW);
	GPIO_write(S1_PORT,S1_PIN,LOW);
	}
	else if(mode == RGB_2_PER){
		// Set Pulse Width scaling to 2%
	GPIO_write(S0_PORT,S0_PIN,LOW);
	GPIO_write(S1_PORT,S1_PIN,HIGH);
	}
	else if(mode == RGB_20_PER){
		// Set Pulse Width scaling to 20%
	GPIO_write(S0_PORT,S0_PIN,HIGH);
	GPIO_write(S1_PORT,S1_PIN,LOW);
	}
	else if(mode == RGB_100_PER){
		// Set Pulse Width scaling to 100%
	GPIO_write(S0_PORT,S0_PIN,HIGH);
	GPIO_write(S1_PORT,S1_PIN,HIGH);
	}
}

void RGBCS_IC_config(void){
	IC_t RGB1;												// Input Capture for echo
	ICAP_init(&RGB1, RGB1_PORT, RGB1_PIN);    		// PB10 as input caputre
 	ICAP_counter_us(&RGB1, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&RGB1, 1, IC_RISE);   	// TIM2_CH1 as IC1 , rising edge detect
	ICAP_setup(&RGB1, 2, IC_FALL);   	// TIM2_CH1 as IC2 , falling edge detect
	
	IC_t RGB2;												// Input Capture for echo
	ICAP_init(&RGB2, RGB2_PORT, RGB2_PIN);    		// PB10 as input caputre
 	ICAP_counter_us(&RGB2, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&RGB2, 1, IC_RISE);   	// TIM2_CH1 as IC1 , rising edge detect
	ICAP_setup(&RGB2, 2, IC_FALL);   	// TIM2_CH1 as IC2 , falling edge detect
	
	IC_t RGB3;												// Input Capture for echo
	ICAP_init(&RGB3, RGB3_PORT, RGB3_PIN);    		// PB10 as input caputre
 	ICAP_counter_us(&RGB3, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&RGB3, 1, IC_RISE);   	// TIM2_CH1 as IC1 , rising edge detect
	ICAP_setup(&RGB3, 2, IC_FALL);   	// TIM2_CH1 as IC2 , falling edge detect
}

void RGBCS_config(int color) {
  switch(color){
		case RGB_R:
			GPIO_write(S2_PORT,S2_PIN,LOW);
			GPIO_write(S3_PORT,S3_PIN,LOW);
			break;
		case RGB_G:
			GPIO_write(S2_PORT,S2_PIN,HIGH);
			GPIO_write(S3_PORT,S3_PIN,HIGH);
			break;
		case RGB_B:
			GPIO_write(S2_PORT,S2_PIN,LOW);
			GPIO_write(S3_PORT,S3_PIN,HIGH);
			break;
		default: 
			GPIO_write(S2_PORT,S2_PIN,HIGH);
			GPIO_write(S3_PORT,S3_PIN,LOW);
			
	}
}

void RGB_OFFSET_SETTING(uint8_t _delivery, uint32_t _RGB_cnt, uint32_t* _rgb_val){
	if(_delivery == '1'){
					if(_RGB_cnt == 0){*_rgb_val += 100;}
					else if(_RGB_cnt == 1){*_rgb_val -= 0;}
					else if(_RGB_cnt == 2){*_rgb_val += 50;}
				}
	// delivery = 2
	else if(_delivery == '2'){
		if(_RGB_cnt == 0){*_rgb_val += 90;}
		else if(_RGB_cnt == 1){*_rgb_val -= 10;}
		else if(_RGB_cnt == 2){*_rgb_val -= 50;}
	}
	// delivery = 3
	else if(_delivery == '3'){
		if(_RGB_cnt == 0){*_rgb_val += 100;}
		else if(_RGB_cnt == 1){*_rgb_val -= 0;}
		else if(_RGB_cnt == 2){*_rgb_val -= 10;}
	}
}
```



- #### ecMCU1.c

```c
#include "stm32f411xe.h"
#include "ecMCU1.h"

//State number 
#define S0 0	// wait
#define S1 1	// Release product R
#define S2 2	// Release product G
#define S3 3	// Release product B
#define S4 4

volatile MCU1_t myPlant; 

typedef struct {
	uint8_t out[3];
  uint32_t next[4];
} State_MCU1_t;

State_MCU1_t FSM_MCU1[4] = {
	// RGBWO (R: Product R, G: Product G, B: Product B,W: print 'waiting for operating', O: print 'operating' )
	{{0,0,0},{S0,S1,S2,S3}}, // s0: wait
	{{1,0,0},{S4,S4,S4,S4}}, // s1: R
	{{0,1,0},{S4,S4,S4,S4}}, // s2: G
	{{0,0,1},{S4,S4,S4,S4}}, // s3: B
	{{0,0,0},{S4,S4,S4,S4}}  // s4: dagie
};

void MCU1_init(PWM_t *PWM1, GPIO_TypeDef* port1, int pin1, PWM_t *PWM2, GPIO_TypeDef* port2, int pin2, PWM_t *PWM3, GPIO_TypeDef* port3, int pin3){
	 
	myPlant.pwm1 	= PWM1;	
	myPlant.port1 = port1;
	myPlant.pin1  = pin1;	
	myPlant.pwm2 	= PWM2;	
	myPlant.port2 = port2;
	myPlant.pin2  = pin2;	
	myPlant.pwm3 	= PWM3;	
	myPlant.port3 = port3;
	myPlant.pin3  = pin3;	
	
	PWM_init_us(myPlant.pwm1, myPlant.port1, myPlant.pin1,SPP);
	PWM_duty(myPlant.pwm1,0.0);
	PWM_init_us(myPlant.pwm2, myPlant.port2, myPlant.pin2,SPP);
	PWM_duty(myPlant.pwm2,0.0);
	PWM_init_us(myPlant.pwm3, myPlant.port3, myPlant.pin3,SPP);
	PWM_duty(myPlant.pwm3,0.0);
}

void SERVO_ONOFF(PWM_t *pwm, int ONOFF){
	if(ONOFF == 1){PWM_duty(pwm,SRD);}
	else					{PWM_duty(pwm,0);}
}

void MCU1_Out(uint32_t state){
	SERVO_ONOFF(myPlant.pwm1, FSM_MCU1[state].out[0]);
	SERVO_ONOFF(myPlant.pwm2, FSM_MCU1[state].out[1]);
	SERVO_ONOFF(myPlant.pwm3, FSM_MCU1[state].out[2]);
	if			(state == 0){printf("\033[H\033[2J"); printf("RGB Input: "); delay_ms(1000);}
	else if	(state == 1){printf("\33[2K\r"); printf("\r\nProduct = RED"); delay_ms(7000);}
	else if	(state == 2){printf("\33[2K\r"); printf("\r\nProduct = GREEN"); delay_ms(7000);}
	else if	(state == 3){printf("\33[2K\r"); printf("\r\nProduct = BLUE"); delay_ms(7000);}
	else if	(state == 4){printf("\r\nWaiting for finishing delivery...\r\n"); delay_ms(1000);}
}

uint32_t MCU1_nextState(uint32_t state){
	return FSM_MCU1[state].next[state];
}

```



- #### **Car(MCU3)**

```c
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : SeungEun Hwang
Created          : 12-07-2022
Modified         : 12-18-2022
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for EC_Final_Project RGB sensor
/----------------------------------------------------------------*/

#include "ecInclude.h"

/*---------------------------------------------------------------------------*/
// define
/*---------------------------------------------------------------------------*/
#define LED_PIN 5
#define END_CHAR 13
#define A 0
#define B 1

#define PIEZO_PIN 	1
#define ILLUM_PIN		0
#define IR_R_PIN 		0
#define IR_L_PIN 		1

#define TRIG_PIN		6		
#define ECHO_PIN		10		

#define PWM_PERIOD	100

#define ILLUML_CUT  1100

#define DELAY_MSEC  20

#define DELIVERY_MSEC 5000
#define STOP_MSEC			10000
/*---------------------------------------------------------------------------*/
// Check sensor varialbe
/*---------------------------------------------------------------------------*/
int seqCHn[4] = {8,9,10,  }; // chn number

uint32_t IR1 = 0;
uint32_t IR2 = 0;
uint32_t ILLUM = 0;
uint32_t PIEZO = 1; //piezoelectric sensor

/*---------------------------------------------------------------------------*/
// flag
/*---------------------------------------------------------------------------*/ 
int flag = 0;

PWM_t dcPwm[2];

enum Num {STOP = 0, 
					DERIVE_1, 
					DELIVERY, 
					DERIVE_2,
					ULTRASONIC
}MODE;

int preMODE = 99;		// mode check before step

/*---------------------------------------------------------------------------*/
// ultra sonic variable
/*---------------------------------------------------------------------------*/
uint32_t ovf_cnt 		= 0;
float distance			= 0.0;
float timeInterval 	= 0.0;
float timeSt 				= 0.0;
float timeEnd 			= 0.0;

float PulseWidth 		= 10;


/*---------------------------------------------------------------------------*/
// motor varialbe
/*---------------------------------------------------------------------------*/
// speed of rc car, motor direction
float 	speed_A = 0.0;
float 	speed_B = 0.0;

int			dir_A 	= HIGH;
int 		dir_B 	= HIGH;

typedef struct {
		GPIO_TypeDef *port;
		int pin; // TIM3 Ch3
}_Pin;

_Pin dcPwmPin[2] = {
	{GPIOA, 8}, // TIM1 ch1
	{GPIOA, 9}	// TIM1 ch2
};

_Pin dcDirPin[2] = {
	{GPIOC, 9}, 	//motor A
	{GPIOC, 8}		//motor B
};

void setup(void);
void EXTI15_10_IRQHandler(void);
void ADC_IRQHandler(void);

/*---------------------------------------------------------------------------*/
// main
/*---------------------------------------------------------------------------*/
int main(void) { 
	
	setup();
	MODE = STOP;	
	printf("strat! \r\n");
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
        
        ////	state check 
        //		printf("PIEZO = %d\r\n",GPIO_read(GPIOA, 0));	
        //		printf("IR1 = %d \r\n",IR1); // right
        //		printf("IR2 = %d \r\n",IR2); // left  
        //		printf("ILLUM = %d \r\n",ILLUM); 			
        //		printf("dis = %f \r\n",(double)distance); 
        //		printf("PIEZO = %d \r\n",PIEZO); // 
        //		printf("preMODE = %d \r\n", preMODE);
        //		printf("MODE = %d \r\n", MODE);
        //		printf("\r\n"); 

        //// piezo sensor
		PIEZO = GPIO_read(GPIOC, PIEZO_PIN);			
		
		switch(MODE){
			
		// stop with out load
		case STOP:	
			PWM_disable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
			PWM_disable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
			
        // piezo got product and timer on then start
			if(PIEZO == 0 && is_TIM_ENA(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin) == 0) {
				preMODE = MODE;
				MODE = DERIVE_1;
					
				PWM_enable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
				PWM_enable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
			}
			break;
			
		// before delivery done derive
		case DERIVE_1:		
			PWM_duty(&dcPwm[A], 1.0 - speed_A);
			PWM_duty(&dcPwm[B], 1.0 - speed_B );
			GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// when motor A is HIGH fo front
			GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// when motor B is HIGH fo front
                
		// delivery spot has lazer so detect the light then stop the car
			if(ILLUM <ILLUML_CUT && PIEZO == 0){
                
                // state update
				preMODE = MODE;	
				MODE = DELIVERY;										
			}
			break;
			
		// Arrive at the delivery spot
		case DELIVERY:
			PWM_disable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
			PWM_disable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
	
			// if light dosen't detect car go back to the factory	
			if(ILLUM > ILLUML_CUT && preMODE == DERIVE_1){
				
                // state update
				preMODE = MODE;
				MODE = DERIVE_2;
				PWM_enable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
				PWM_enable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);

			}
			break;

		// after delivery derive
		case DERIVE_2:
			PWM_duty(&dcPwm[A], 1.0 - speed_A);
			PWM_duty(&dcPwm[B], 1.0 - speed_B );
			GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, dir_A);			// A는 HIGH일 때 전진
			GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, dir_B);			// B는 HIGH일 때 전진		
		
			if((distance > 2 && distance < 15)){
					preMODE = MODE;
					MODE = ULTRASONIC;
			}
			break;

		// 다시 처음 정지
		case ULTRASONIC:	
			PWM_disable(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
			PWM_disable(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);	
			delay_ms(STOP_MSEC);
			preMODE = MODE;			
			MODE = STOP;
			break;
		}	
		
		delay_ms(DELAY_MSEC);
	}
}
// Initialiization  
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_ms_init(1);
	USART_init(USART2, 9600);
	
	// internal LED set
	GPIO_out_set(GPIOA, LED_PIN, PULLUP, MSPEED, PUSHPULL);
	
	// Piezo sensor
	GPIO_in_set(GPIOC, PIEZO_PIN, PULLUP);
	
	// IR sensor
	ADC_INJ_init(GPIOB, IR_R_PIN, TRGO);
	ADC_INJ_init(GPIOB, IR_L_PIN, TRGO);
	
	// Illimination sensor
	ADC_INJ_init(GPIOC, ILLUM_PIN, TRGO);
	
	// ADC injection sequence
	ADC_INJ_sequence(3, seqCHn); 
	ADC_INJ_start();
	
	// pwm dir setting
	for (int i = 0; i < 2; i++){
		GPIO_out_set(dcDirPin[i].port, dcDirPin[i].pin, PULLDO,HSPEED,PUSHPULL);
	}
	
	// PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin, int pupd, int speed, int type ,int dir, uint32_t msec);
	PWM_init(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin, FSPEED, PULLDO, PUSHPULL, UPPCOUNT,1);
	PWM_init(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin, FSPEED, PULLDO, PUSHPULL, UPPCOUNT,1);
	PWM_period_us(&dcPwm[A], PWM_PERIOD);
	PWM_period_us(&dcPwm[B], PWM_PERIOD);
	
	// speed control 
	PWM_duty(&dcPwm[A], 1 - speed_A);
	PWM_duty(&dcPwm[B], 1 - speed_B );
	
	// A는 HIGH일 때 전진
	GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, HIGH);
	// B는 HIGH일 때 전진
	GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, HIGH);
	

	
	// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;												// PWM1 for trig
	PWM_init(&trig, GPIOA, TRIG_PIN, NOPUPD, FSPEED, PUSHPULL, UPPCOUNT, 1);	// PA_6: Ultrasonic trig pulse TIM3 ch1
	PWM_period_us(&trig, 50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig, 10);   	// PWM pulse width of 10us
	
	// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;												// Input Capture for echo
	ICAP_init(&echo, GPIOB, ECHO_PIN);   	// PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, IC_RISE);   	// TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, IC_FALL);   	// TIM2_CH3 as IC4 , falling edge detect	
	
	// NVIC Priority setting
	NVIC_SetPriority(ADC_IRQn, 2);						    	
	NVIC_EnableIRQ(ADC_IRQn);									  
	
	NVIC_SetPriority(TIM2_IRQn, 1);		// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);				// TIM2 interrupt request enable
	
	
}

void ADC_IRQHandler(void){
	
	if(is_ADC_JEOC()){       //after finishing sequence
		
		IR1 = ADC1->JDR3;
		IR2 = ADC1->JDR2;
		ILLUM = ADC1->JDR1;
		
		clear_ADC_JEOC();
	}
	

	if (IR1 > 2000 && IR2 < 2000) { 
		// 직진 우회전
		speed_A = 0.95;
		speed_B = 0.6;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}			
	else if (IR2 > 2000 && IR1 < 2000) {
		// 직진 좌회전
		speed_A = 0.6;
		speed_B = 0.95 ;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}else {
		speed_A = 0.7;
		speed_B = 0.7;  
		dir_A		= HIGH;
		dir_B		= HIGH;
	}
		
	if(ILLUM <ILLUML_CUT && MODE == DERIVE_1){
		speed_A = 0.0;
		speed_B = 0.0;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}
	
	if((distance > 2 && distance < 15) &&  MODE == DERIVE_2){
		speed_A = 0.0;
		speed_B = 0.0;
		dir_A		= HIGH;
		dir_B		= HIGH;
	}

}

// ultra sonic
void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		ovf_cnt++;														// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 3)){ 									// TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
		timeSt = TIM2->CCR3;									// Capture TimeStart
		clear_CCIF(TIM2, 3);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2, 4)){ 							// TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
		timeEnd = TIM2->CCR4;									// Capture TimeEnd
		clear_CCIF(TIM2, 4);								  // clear capture/compare interrupt flag 
		
		timeInterval = PulseWidth*((timeEnd - timeSt)+(ovf_cnt * 0xFFFF)); 					// Total time of echo pulse (10us * counter pulses -> [msec] unit)
		distance = (float) timeInterval / 58; 	// [mm] -> [cm]
				
		ovf_cnt = 0;                        	// overflow reset	
	}	
}



```



## VII. Discussion and Analysis

**Cho HanSe**: 

- **Using almost all the parts which learned in Embedded Controller class**

  We use all of the contents below.

  - GPIO - digital I/O, analong input, AF, etc.
  - SYSTICK - delay function
  - TIMER_PWM - Servo motor, Adjust LED brightness
  - INPUT CAPTURE - object detection(Ultra sonic sensor), RGB sensor output frequency detect
  - USART - VCP(Virtual COM Port)(MCU1 - COM), Zigbee(MCU1 - MCU2)
  - ADC(injection) - IR sensor, Illumination sensor

- **RGB Sensor part**

  ​	RGB sensor has each color(red, green, blue) detection mode. Each mode has another default output frequency when detect each pure color(e.q. RED(255, 0, 0), GREEN(0, 255, 0), BLUE(0, 0, 255) ). If you want to separate three color, you should not use threshold value of each color mode output frequency similarly to using IR sensor(above). You should correct offset of three color output frequency. You should compare the three output frequency(R,G,B). 

  ​	We have to use LED for sensing RGB in dark place, but if LED is too bright, it detect all color to white. So we have to find appropriate pwm duty for LED's brightness.

  ​	In MCU2 part(Delivery destination), when car arrive slowly, first detected color might be trash value. so when RGB sensor start to detect color, wait few second or few cycle or RGB sensing. 

- **Communication Part**

  When start this project, we used Bluetooth, but then I used two terminals(mcu1(vcp), muc2(bluetooth)). In order to make the computer use only one terminal, connect the computer and mcu1 with usb port, connect mcu1 and mcu2 through zigbee communication, and transmit the goods and delivery information in the order of com -> mcu1 -> mcu2.

- **Development**

  ​	We turned on(enable) all three RGB sensors in MCU2 and used them. This setting use 3 Timer(Timer 2, 3, 4) for  each sensor. The better way is as follows: With just one timer on(enable), when I received the product and delivery address information from MCU1, if you turn on the RGB sensor of a specific delivery destination, you can only use the sensor's data.

  ​	Add: 

  		1. In this project, when an unwanted item arrives at the destination, The 'fail' message is sent to the destination and ends. But what I want is, when a fail message arrives to MCU1, MCU1(Factory) send the item back to the delivery address.
  		2. You should input the product and delivery address in the computer terminal, we want to have you enter the information on your cell phone.



**Hwang SeungEun**:  

The car we built has several sensors in one MCU. Four types of sensors were used: Ultra sonic, IR sensor, illumination sensor, and piezoelectric sensor.

The piezoelectric sensor had no problem with other sensors because it received digital signals, but IR sensors and illumination sensors, which had to receive analog values, could have a twisted order in the general analog values learned in class, and it was difficult to find where the problem occurred because each sensor had a different operating cycle. So I used ADC Injection mode. 

ADC Injection mode has its own registry so that several analog signals are not confused, so there is no need to worry about the order being confused like regular ADC.

And the priority between ultrasonic and IR sensors is also important. The operation depends on the priority of the interrupt of each sensor. Therefore, the priority of the ultrasonic sensor with a small operating frequency was set to 1 and the priority of the IR sensor, which continuously recognizes the value, was set to 2. The ultrasonic sensor does not work if the priorities of the two are changed.

The DC motor output power was not linear and the dead zone size was random, so it was changed to a motor with good performance. 

Finally, the condition of the auxiliary battery is important. When the battery was not enough, it often did not work because it could not supply power to several sensors. In particular, in this case, the ultrasonic sensor did not operate.



## VIII. Reference

[EC gitbook](https://ykkim.gitbook.io/ec/)

[rgb sensor site](https://create.arduino.cc/projecthub/SurtrTech/color-detection-using-tcs3200-230-84a663)

