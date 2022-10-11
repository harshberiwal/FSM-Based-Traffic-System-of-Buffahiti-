/*
 * test.c includes all the definitions of functions declared in test.h.
 *
 * It includes the Int Main().
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#include "test.h"

#define CROSSWALK_TIME  (160 + TRANSITION_TIME)
#define CROSSWALK_OFF	4
#define CROSSWALK_ON	16

#define TICKS 			(187500 -1)
#define TIME_OFFSET 	1100


/*states are enum*/
typedef enum {
	STOP,
	GO,
	WARNING,
	CROSSWALK,
} states_t;

typedef void (*action_t)(int, uint32_t);
int last_state =0;
void Signal_Yellow_on(int last_st, uint32_t last_col);
void Signal_Red_on(int last_st, uint32_t last_col);
void Signal_Green_on(int last_st, uint32_t last_col);
void Signal_Blue_on(int last_st, uint32_t last_col);
void state_check(uint16_t timeout_time_elapsed);
int get_curr_val(uint32_t req_color, uint32_t last_color, uint32_t LED_Type, uint8_t shift);
uint32_t update_last_val();
void initialize_SysTick();

#define STOP_VAL 		0x611E3C
#define GO_VAL   		0x229622
#define WARNING_VAL  	0xFFB200
#define CROSSWALK_VAL	0x001030

#define RED_CHECK 	0XFF0000
#define GREEN_CHECK 0X00FF00
#define BLUE_CHECK 	0x0000FF

#define RED_SHIFT 		16
#define GREEN_SHIFT 	8
#define BLUE_SHIFT  	0
#define ONE_SEC 		16

typedef uint32_t ticktime_t;

typedef enum{
	Red,
	Green,
	Blue,
}CntVal;


typedef struct {
	states_t TIMEOUT;
	states_t BUTTON_PRESSED;
	uint16_t time_elapsed;
	uint32_t color;
	action_t take_action;
} state_table_t;


state_table_t state_array[]=
{ /*STOP*/ 		{GO, CROSSWALK, STOP_TIME, STOP_VAL, Signal_Red_on},
  /*GO*/		{WARNING, CROSSWALK, GO_TIME, GO_VAL, Signal_Green_on},
  /*WARNING*/	{STOP, CROSSWALK, WARNING_TIME, WARNING_VAL, Signal_Yellow_on},
  /*CROSSWALK*/ {GO, CROSSWALK, CROSSWALK_TIME, CROSSWALK_VAL, Signal_Blue_on},
};

int Timeout_time = 0;
int state =0, new_state =0;
uint16_t touch_check =0;
bool touch_status= false ;
int total_interrupt = 0;
uint32_t current_time =0;
uint32_t past_time =0;
uint32_t time_now =0;
uint32_t last_val = STOP_VAL;
uint32_t curr_val_R =0, curr_val_G =0, curr_val_B =0;


int main(void) {

    //**********************Init board hardware***************************
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    //*********************Init FSL debug console*************************
    BOARD_InitDebugConsole();
#endif
    Init_TPM(256);
	//gpio_init();
	initialize_SysTick();
	//startup_sequence();
	Touch_Init();
	LOG("Main Loop is starting\n");
    while(1) {
    	touch_check = Touch_Poll();
    	if(touch_check > 480)
    		touch_status = true;
    	state_check(Timeout_time);
    }
}

void initialize_SysTick(){
	//************Initialize System Timer with the Required Ticks of 4.8 MHz********
	SysTick->LOAD = TICKS;
	NVIC_SetPriority (SysTick_IRQn ,3);
	SysTick->VAL  = 0;
	SysTick->CTRL = 0b0011;
}

void SysTick_Handler(void)
{
	Timeout_time++;
	total_interrupt++;
}

ticktime_t now()
{
	return (total_interrupt);
}

void reset_timer()
{
	SysTick -> VAL =0;
}

void state_check(uint16_t timeout_time_elapsed)
{
	if(timeout_time_elapsed == state_array[state].time_elapsed){
		reset_timer();
		current_time = now();
		time_now = (current_time * 62.5);
		last_val = state_array[state].color;
		new_state = state_array[state].TIMEOUT;
		LOG("State Transitioned from %d to %d at %d msec\n", state, new_state, time_now);
		last_state = state;
		state = new_state;
		Timeout_time = 0;
	}
	else if(touch_status && (state != CROSSWALK)) {
		reset_timer();
		new_state = state_array[state].BUTTON_PRESSED;
		last_val  = update_last_val();
		current_time = now();
		time_now = (current_time * 62.5);
		LOG("Button Pressed Detected at %d msec\n", time_now);
		state = new_state;
		Timeout_time =0;
		touch_status = false;
	}
	else{
		touch_status = false;
	}
	state_array[new_state].take_action(last_state,last_val);
}

void Signal_Red_on(int last_st,uint32_t last_col)
{
	if(Timeout_time<=ONE_SEC)
	{
		curr_val_R = get_curr_val(STOP_VAL, last_col, RED_CHECK, RED_SHIFT);
		curr_val_G = get_curr_val(STOP_VAL, last_col, GREEN_CHECK, GREEN_SHIFT);
		curr_val_B = get_curr_val(STOP_VAL, last_col, BLUE_CHECK, BLUE_SHIFT);
		TPM2->CONTROLS[0].CnV = curr_val_R;
		TPM2->CONTROLS[1].CnV = curr_val_G;
		TPM0->CONTROLS[1].CnV = curr_val_B;
	}
	else
	{
		TPM2->CONTROLS[0].CnV = (STOP_VAL & RED_CHECK)	 >> RED_SHIFT;
		TPM2->CONTROLS[1].CnV = (STOP_VAL & GREEN_CHECK) >> GREEN_SHIFT;
		TPM0->CONTROLS[1].CnV = (STOP_VAL & BLUE_CHECK)  >> BLUE_SHIFT;
	}

}

void Signal_Green_on(int last_st, uint32_t last_col)
{
	if(Timeout_time<=ONE_SEC)
	{
		curr_val_R = get_curr_val(GO_VAL, last_col, RED_CHECK, RED_SHIFT);
		curr_val_G = get_curr_val(GO_VAL, last_col, GREEN_CHECK, GREEN_SHIFT);
		curr_val_B = get_curr_val(GO_VAL, last_col, BLUE_CHECK, BLUE_SHIFT);
		TPM2->CONTROLS[0].CnV = curr_val_R;
		TPM2->CONTROLS[1].CnV = curr_val_G;
		TPM0->CONTROLS[1].CnV = curr_val_B;
	}
	else
	{
		TPM2->CONTROLS[0].CnV = (GO_VAL & RED_CHECK)   >> RED_SHIFT;
		TPM2->CONTROLS[1].CnV = (GO_VAL & GREEN_CHECK) >> GREEN_SHIFT;
		TPM0->CONTROLS[1].CnV = (GO_VAL & BLUE_CHECK)  >> BLUE_SHIFT;
	}
}


void Signal_Blue_on(int last_st, uint32_t last_col)
{
	if(Timeout_time <= ONE_SEC) {
		curr_val_R = get_curr_val(CROSSWALK_VAL, last_col, RED_CHECK, RED_SHIFT);
		curr_val_G = get_curr_val(CROSSWALK_VAL, last_col, GREEN_CHECK, GREEN_SHIFT);
		curr_val_B = get_curr_val(CROSSWALK_VAL, last_col, BLUE_CHECK, BLUE_SHIFT);
		TPM2->CONTROLS[0].CnV = curr_val_R;
		TPM2->CONTROLS[1].CnV = curr_val_G;
		TPM0->CONTROLS[1].CnV = curr_val_B;
		past_time = Timeout_time;
	}
	else if((Timeout_time - past_time) < CROSSWALK_OFF) {
		TPM2->CONTROLS[0].CnV = 0;
		TPM2->CONTROLS[1].CnV = 0;
		TPM0->CONTROLS[1].CnV = 0;
	}
	else if((Timeout_time - past_time) >= CROSSWALK_ON) {
		past_time = Timeout_time;
	}
	else {
		TPM2->CONTROLS[0].CnV = (CROSSWALK_VAL & RED_CHECK)   >> RED_SHIFT;
		TPM2->CONTROLS[1].CnV = (CROSSWALK_VAL & GREEN_CHECK) >> GREEN_SHIFT;
		TPM0->CONTROLS[1].CnV = (CROSSWALK_VAL & BLUE_CHECK)  >> BLUE_SHIFT;
	}
}

void Signal_Yellow_on(int last_st, uint32_t last_col)
{
	if(Timeout_time<=ONE_SEC)
	{
		curr_val_R = get_curr_val(WARNING_VAL, last_col, RED_CHECK, RED_SHIFT);
		curr_val_G = get_curr_val(WARNING_VAL, last_col, GREEN_CHECK, GREEN_SHIFT);
		curr_val_B = get_curr_val(WARNING_VAL, last_col, BLUE_CHECK, BLUE_SHIFT);
		TPM2->CONTROLS[0].CnV = curr_val_R;
		TPM2->CONTROLS[1].CnV = curr_val_G;
		TPM0->CONTROLS[1].CnV = curr_val_B;
	}
	else
	{
		TPM2->CONTROLS[0].CnV = (WARNING_VAL & RED_CHECK)	>> RED_SHIFT;
		TPM2->CONTROLS[1].CnV = (WARNING_VAL & GREEN_CHECK) >> GREEN_SHIFT;
		TPM0->CONTROLS[1].CnV = (WARNING_VAL & BLUE_CHECK)  >> BLUE_SHIFT;
	}
}


int get_curr_val(uint32_t req_color, uint32_t last_color, uint32_t LED_Type, uint8_t shift)
{
	int calc_val =0;
	int des_clr =0, curr_clr = 0;
	des_clr = ((req_color & LED_Type)>>shift);
	curr_clr = ((last_color & LED_Type)>>shift);
	calc_val = ((des_clr-curr_clr)*(Timeout_time*0.0625))+curr_clr;
	return calc_val;
}

uint32_t update_last_val()
{
	return ((curr_val_R <<RED_SHIFT) | (curr_val_G <<GREEN_SHIFT) | curr_val_B);
}








