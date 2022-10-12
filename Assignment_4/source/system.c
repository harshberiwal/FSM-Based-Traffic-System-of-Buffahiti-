/*
 * system.c includes all the definition of initialzing the SysTick
 * This include initialize_Systick(), get_timer(), reset_timer()
 * and the Systick Handler which replaces the Weak SystickIRQhandler
 * definition in the Startup file.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
*/


#include "system.h"

#define SYSTICK_CONTROL 3
#define LEAST_PRIORITY  3
#define RESET			0
#define TICKS 			(187500 -1)
#define INT_TIME  		62.5

uint32_t total_interrupt = 0, Timeout_Time = 0;

void initialize_SysTick()
{
	//***********Initialize System Timer with the Required Ticks of 62.5ms*********
	SysTick->LOAD = TICKS;
	NVIC_SetPriority (SysTick_IRQn ,LEAST_PRIORITY);
	SysTick->VAL  = RESET;
	SysTick->CTRL = SYSTICK_CONTROL;
}

ticktime_t now()
{
	return (total_interrupt*INT_TIME);
}

void reset_timer()
{
	SysTick -> VAL = RESET;
	Timeout_Time =RESET;
}

ticktime_t get_timer()
{
	return Timeout_Time;
}

void SysTick_Handler(void)
{
	Timeout_Time++;
	total_interrupt++;
}
