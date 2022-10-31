#include "ecInclude.h"



// Stepper Motor function
uint32_t step_delay = 100; 
uint32_t step_per_rev = 64*32;
	 
// Stepper Motor variable
volatile Stepper_t myStepper; 

//FULL stepping sequence  - FSM
typedef struct {
	uint8_t out;
  uint32_t next[2];
} State_full_t;

	
State_full_t FSM_full[4] = {  
//AA'BB'
	
 {0b1010,{S1,S3}},
 {0b0110,{S2,S0}},
 {0b0101,{S3,S1}},
 {0b1001,{S0,S2}}

};

//HALF stepping sequence
typedef struct {
	uint8_t out;
  uint32_t next[2];
} State_half_t;

State_half_t FSM_half[8] = { 
	
 {0b1000,{S1,S7}},  // s0
 {0b1010,{S2,S0}},	// s1
 {0b0010,{S3,S1}},	// s2
 {0b0110,{S4,S2}},	// s3
 {0b0100,{S5,S3}},	// s4
 {0b0101,{S6,S4}},	// s5
 {0b0001,{S7,S5}},	// s6
 {0b1001,{S0,S6}}		// s7
};

void Stepper_init(GPIO_TypeDef* port1, int pin1, 
									GPIO_TypeDef* port2, int pin2, 
									GPIO_TypeDef* port3, int pin3, 
									GPIO_TypeDef* port4, int pin4)
										{
	 
//  GPIO Digital Out Initiation
	// A = 1
	myStepper.port1 = port1;
  myStepper.pin1  = pin1;
	// A' = 2
	myStepper.port2 = port2;
  myStepper.pin2  = pin2;
	// B = 3
	myStepper.port3 = port3;
  myStepper.pin3  = pin3;
	// B' = 4
	myStepper.port4 = port4;
  myStepper.pin4  = pin4;
	
	//  GPIO Digital Out Initiation
	// No pull-up Pull-down , Push-Pull, Fast	
	// Port1,Pin1 ~ Port4,Pin4
	GPIO_out_set(myStepper.port1, myStepper.pin1, NOPUPD, FSPEED, PUSHPULL);
	GPIO_out_set(myStepper.port2, myStepper.pin2, NOPUPD, FSPEED, PUSHPULL);
	GPIO_out_set(myStepper.port3, myStepper.pin3, NOPUPD, FSPEED, PUSHPULL);
	GPIO_out_set(myStepper.port4, myStepper.pin4, NOPUPD, FSPEED, PUSHPULL);
}


void Stepper_setSpeed (long whatSpeed, int mode){      // rpm
	
	if(mode == FULL){
		step_delay = 	(60000*1000) /  (GEAR_RATIO * FULL_REV * whatSpeed);
	}
	else if(mode == HALF){
		
		step_delay = 	(60000*1000) /  (GEAR_RATIO * HALF_REV * whatSpeed);
	}
	
		delay_ms(step_delay);
	
}


void Stepper_step(int steps, int dir, int mode, long rpm){
	 uint32_t state = 0; // 여기서 시작 state를 0으로 시작함 --> S0에서 시작함
	 myStepper._step_num = steps;

	 for(; myStepper._step_num > 0; myStepper._step_num--){ // run for step size
				Stepper_setSpeed (rpm, mode);                                		// delay (step_delay); 
					 
		    if (mode == FULL) 		 												
						state = FSM_full[state].next[dir];// YOUR CODE       // state = next state
				else if (mode == HALF) 
						state = FSM_half[state].next[dir];// YOUR CODE       // state = next state
				
				Stepper_pinOut(state, mode);
   }
}

void Stepper_pinOut (uint32_t state, int mode){
	
	if (mode == FULL){         // FULL mode
		GPIO_write(myStepper.port1, myStepper.pin1, FSM_full[state].out >> 3 & 1);
		GPIO_write(myStepper.port2, myStepper.pin2, FSM_full[state].out >> 2 & 1);
		GPIO_write(myStepper.port3, myStepper.pin3, FSM_full[state].out >> 1 & 1);
		GPIO_write(myStepper.port4, myStepper.pin4, FSM_full[state].out >> 0 & 1);

	}	 
	else if (mode == HALF){    // HALF mode
		GPIO_write(myStepper.port1, myStepper.pin1, FSM_half[state].out >> 3 & 1);
		GPIO_write(myStepper.port2, myStepper.pin2, FSM_half[state].out >> 2 & 1);
		GPIO_write(myStepper.port3, myStepper.pin3, FSM_half[state].out >> 1 & 1);
		GPIO_write(myStepper.port4, myStepper.pin4, FSM_half[state].out >> 0 & 1);
	}
}

void Stepper_stop (void){ 
     
    myStepper._step_num = 0;    
			// All pins(Port1~4, Pin1~4) set as DigitalOut '0'
		GPIO_write(myStepper.port1, myStepper.pin1, myStepper._step_num);
		GPIO_write(myStepper.port2, myStepper.pin2, myStepper._step_num);
		GPIO_write(myStepper.port3, myStepper.pin3, myStepper._step_num);
		GPIO_write(myStepper.port4, myStepper.pin4, myStepper._step_num);
	
}

