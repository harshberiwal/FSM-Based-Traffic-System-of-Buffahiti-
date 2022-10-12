/*
 * state_machine.c includes all the definitions of functions declared in
 * state_machine.h.
 * This includes
 * state_check(uint16_t),
 * Signal_Red_on(int uint32_t);
 * Signal_Green_on(int, uint32_t);
 * Signal_Yellow_on(int, uint32_t);
 * Signal_Blue_on(int, uint32_t);
 * get_curr_val(uint32_t, uint32_t, uint32_t, uint8_t);
 * uint32_t update_last_val();
 * void transitions(uint32_t, uint32_t);
 *
 * We used A Table Driven Approach for the State Machine which made the State_check
 * code concise and easy to follow.
 *
 * A function pointer was used to avoid repetitive code in the state_check.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#include "state_machine.h"

#define CROSSWALK_TIME  (160 + TRANSITION_TIME)
#define CROSSWALK_OFF	4
#define CROSSWALK_ON	16
#define STOP_VAL 		0x611E3C
#define GO_VAL   		0x229622
#define WARNING_VAL  	0xFFB200
#define CROSSWALK_VAL	0x001030
#define RED_CHECK 		0XFF0000
#define GREEN_CHECK 	0X00FF00
#define BLUE_CHECK 		0x0000FF
#define RED_SHIFT 		16
#define GREEN_SHIFT 	8
#define BLUE_SHIFT  	0
#define ONE_SEC 		16
#define FIRST_CHANNEL   0
#define SECOND_CHANNEL  1
#define OFF				0
#define PCT				0.0625

//******************************STATES ARE ENUM*********************************
typedef enum {
	STOP,
	GO,
	WARNING,
	CROSSWALK,
} states_t;

//************DELCARING FUNCTION POINTER WITH STATE AND LAST_COLOR**************
typedef void (*action_t)(int, uint32_t);

typedef uint32_t ticktime_t;

//*********************STATE SPECIFIC MEMORY ELEMENTS***************************
typedef struct {
	states_t TIMEOUT;
	states_t BUTTON_PRESSED;
	uint16_t time_elapsed;
	uint32_t color;
	char *arr;
	action_t take_action;
} state_table_t;

//**************STATE TABLE ARRAY WITH STATE SPECIFIC ELEMENTS******************
state_table_t state_array[]=
{ /*STOP*/ 		{GO, CROSSWALK, STOP_TIME, STOP_VAL, "STOP", Signal_Red_on},
  /*GO*/		{WARNING, CROSSWALK, GO_TIME, GO_VAL, "GO", Signal_Green_on},
  /*WARNING*/	{STOP, CROSSWALK, WARNING_TIME, WARNING_VAL, "WARNING",Signal_Yellow_on},
  /*CROSSWALK*/ {GO, CROSSWALK, CROSSWALK_TIME, CROSSWALK_VAL,"CROSSWALK", Signal_Blue_on},
};

int last_state =0, state =0, new_state =0;
int touch_check =0;
uint32_t timeout_time =0, past_time =0,time_now=0;
uint32_t curr_val_R =0, curr_val_G =0, curr_val_B =0;
bool touch_status= false,button_status =false;
uint32_t last_val = STOP_VAL;

void state_check(uint16_t timeout_time_elapsed)
{
	timeout_time = timeout_time_elapsed;

	//******************************CHECKING TOUCH STATUS**********************************
	touch_status = Touch_Poll();
	button_status = button_check();
	if(timeout_time_elapsed == state_array[state].time_elapsed) {
		//**************************CHECKING CURRENT TIME**********************************
		time_now = now();
		last_val = state_array[state].color;
		//*********************CHANGING STATE AS PER THE STATE TABLE***********************
		new_state = state_array[state].TIMEOUT;
		LOG("State Transitioned from %s to %s at %d msec\n\r", state_array[state].arr,
				state_array[new_state].arr, time_now);
		last_state = state;
		state = new_state;
		//************************RESEETING TIME FOR RE_CALLBACK***************************
		reset_timer();
	}
	else if((touch_status || button_status) && (state != CROSSWALK)) {
		//***********************CHANGING STATE TO CROSSWALK*******************************
		new_state = state_array[state].BUTTON_PRESSED;
		last_val  = update_last_val();
		//**************************CHECKING CURRENT TIME**********************************
		time_now = now();
		LOG("Button Pressed Detected at %d msec\n\r", time_now);
		state = new_state;
		touch_status = false;
		button_status = false;
		//************************RESEETING TIME FOR RE_CALLBACK***************************
		reset_timer();
	}
	else{
		//********************IGNORING SWITCH PRESS DURING CROSSWALK***********************
		button_status = false;
		touch_status = false;
	}
	//********************CALLING FUNC POINTER ACC. TO CURRENT STATE***********************
	state_array[new_state].take_action(last_state,last_val);
}

int get_curr_val(uint32_t req_color, uint32_t last_color, uint32_t LED_Type, uint8_t shift)
{
	int calc_val =0;
	int des_clr =0, curr_clr = 0;
	//********************************GET DESIRED COLOR************************************
	des_clr = ((req_color & LED_Type)>>shift);
	//********************************GET CURRENT COLOR************************************
	curr_clr = ((last_color & LED_Type)>>shift);
	calc_val = ((des_clr-curr_clr)*(timeout_time*PCT))+curr_clr;
	//************************RETURN REQUIRED TRANSITION COLOR*****************************
	return calc_val;
}

uint32_t update_last_val()
{
	return ((curr_val_R <<RED_SHIFT) | (curr_val_G <<GREEN_SHIFT) | curr_val_B);
}

void transitions(uint32_t Curr_Color, uint32_t last_color)
{
	//**************CHECKS AND UPDATE TPM CnV REGISTERS WITH CURRENT TRANSITIONS***********
	if(timeout_time<=ONE_SEC) {
		curr_val_R = get_curr_val(Curr_Color, last_color, RED_CHECK, RED_SHIFT);
		curr_val_G = get_curr_val(Curr_Color, last_color, GREEN_CHECK, GREEN_SHIFT);
		curr_val_B = get_curr_val(Curr_Color, last_color, BLUE_CHECK, BLUE_SHIFT);
		TPM2->CONTROLS[FIRST_CHANNEL].CnV = curr_val_R;
		TPM2->CONTROLS[SECOND_CHANNEL].CnV = curr_val_G;
		TPM0->CONTROLS[SECOND_CHANNEL].CnV = curr_val_B;
	}
	//**************CHECKS AND UPDATE TPM CnV REGISTERS WITH LAST TRANSITION***************
	else {
		TPM2->CONTROLS[FIRST_CHANNEL].CnV = (Curr_Color & RED_CHECK)   >> RED_SHIFT;
		TPM2->CONTROLS[SECOND_CHANNEL].CnV = (Curr_Color & GREEN_CHECK) >> GREEN_SHIFT;
		TPM0->CONTROLS[SECOND_CHANNEL].CnV = (Curr_Color & BLUE_CHECK)  >> BLUE_SHIFT;
	}
}

void Signal_Red_on(int last_st,uint32_t last_col)
{
	transitions(STOP_VAL, last_col);
}

void Signal_Green_on(int last_st, uint32_t last_col)
{
	transitions(GO_VAL, last_col);
}

void Signal_Yellow_on(int last_st, uint32_t last_col)
{
	transitions(WARNING_VAL, last_col);
}

void Signal_Blue_on(int last_st, uint32_t last_col)
{
	//**************CHECKS AND UPDATE TPM CnV REGISTERS WITH CURRENT TRANSITIONS***********
	if(timeout_time <= ONE_SEC) {
		curr_val_R = get_curr_val(CROSSWALK_VAL, last_col, RED_CHECK, RED_SHIFT);
		curr_val_G = get_curr_val(CROSSWALK_VAL, last_col, GREEN_CHECK, GREEN_SHIFT);
		curr_val_B = get_curr_val(CROSSWALK_VAL, last_col, BLUE_CHECK, BLUE_SHIFT);
		TPM2->CONTROLS[FIRST_CHANNEL].CnV = curr_val_R;
		TPM2->CONTROLS[SECOND_CHANNEL].CnV = curr_val_G;
		TPM0->CONTROLS[SECOND_CHANNEL].CnV = curr_val_B;
		past_time = timeout_time;
	}
	//*********************************250 MSEC OFF TIME***********************************
	else if((timeout_time - past_time) < CROSSWALK_OFF) {
		TPM2->CONTROLS[FIRST_CHANNEL].CnV = OFF;
		TPM2->CONTROLS[SECOND_CHANNEL].CnV = OFF;
		TPM0->CONTROLS[SECOND_CHANNEL].CnV = OFF;
	}
	//*********************************750 MSEC ON TIME************************************
	else if((timeout_time - past_time) >= CROSSWALK_ON) {
		past_time = timeout_time;
	}
	//**************CHECKS AND UPDATE TPM CnV REGISTERS WITH LAST TRANSITION***************
	else {
		TPM2->CONTROLS[FIRST_CHANNEL].CnV = (CROSSWALK_VAL & RED_CHECK)   >> RED_SHIFT;
		TPM2->CONTROLS[SECOND_CHANNEL].CnV = (CROSSWALK_VAL & GREEN_CHECK)>> GREEN_SHIFT;
		TPM0->CONTROLS[SECOND_CHANNEL].CnV = (CROSSWALK_VAL & BLUE_CHECK) >> BLUE_SHIFT;
	}
}






