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