/*

 * system.c includes all the definition of initialzing the SysTick
 * This include initialize_Systick()
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *



#include "system.h"

void initialize_SysTick(){
	//************Initialize System Timer with the Required Ticks of 4.8 MHz********
	SysTick->LOAD = TICKS;
	NVIC_SetPriority (SysTick_IRQn ,3);
	SysTick->VAL  = 0;
	SysTick->CTRL = 0b0011;
}

void SysTick_Handler(void)
{
	ticks_transition++;
}

int get_timer()
{
	return ticks_transition;
}


*/
