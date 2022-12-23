#include "ecInclude.h"


void RGBCS_init(void){
	// CS(color sensor) (S0,S1),(S2,S3),CS_EN
	GPIO_out_set(S0_PORT,S0_PIN, PULLUP, MSPEED, PUSHPULL);
	GPIO_out_set(S1_PORT,S1_PIN, PULLUP, MSPEED, PUSHPULL);
	GPIO_out_set(S2_PORT,S2_PIN, PULLUP, MSPEED, PUSHPULL);
	GPIO_out_set(S3_PORT,S3_PIN, PULLUP, MSPEED, PUSHPULL);
	GPIO_out_set(CS_EN_PORT,CS_EN_PIN, PULLUP, MSPEED, PUSHPULL);

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

// Function to configuration Red Pulse Widths
void RGBCS_Red_config(void) {
  // Set sensor to read Red only
  GPIO_write(S2_PORT,S2_PIN,LOW);
	GPIO_write(S3_PORT,S3_PIN,LOW);
}

// Function to configuration Red Pulse Widths
void RGBCS_Green_config(void) {
  // Set sensor to read Red only
  GPIO_write(S2_PORT,S2_PIN,HIGH);
	GPIO_write(S3_PORT,S3_PIN,HIGH);
}

// Function to configuration Red Pulse Widths
void RGBCS_Blue_config(void) {
  // Set sensor to read Red only
  GPIO_write(S2_PORT,S2_PIN,LOW);
	GPIO_write(S3_PORT,S3_PIN,HIGH);
}