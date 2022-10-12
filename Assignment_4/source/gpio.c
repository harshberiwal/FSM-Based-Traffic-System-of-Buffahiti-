/*
 * gpio.c includes all the definitions of functions in gpio.h
 * This include gpio_init() function for the Button Pressed.
 *
 * The IRQ Handler is enabled so when the external button is pressed
 * at Port A Pin 13, An interrupt is generated.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code
 */

#include "gpio.h"

#define MUX_GPIO 		(uint32_t)(0x00000100)
#define INPUT_PIN		13
#define PIN_13			((uint32_t)(1 << 13))
#define IFSR_LR_RET 	0xFFFFFFF
#define GPIO_INIT 		1
#define RISING_EDGE		0x09

bool button_pressed = false;
bool status = false;

void gpio_init()
{
	//****************Turning on Clock for GPIOs****************************
	SIM -> SCGC5 |= PORT_A;

	//****************Configuring Pins as GPIO******************************
	PORTA -> PCR[INPUT_PIN] = PORT_PCR_MUX(GPIO_INIT) | PORT_PCR_IRQC(RISING_EDGE)
								|PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
	//****************Configuring Pins as output****************************
	GPIOB -> PDDR &= ~PIN_13;

	//***************CLEARING ALL PENDING IRQ AND ENABLING IT***************
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	//**************ALL NON CORE INTERRUPTS REQUIRE NVIC********************
	NVIC_EnableIRQ(PORTA_IRQn);
}

//********************ISFR IS SET TO MAX TO CLEAR THE IRQ*******************
void PORTA_IRQHandler(void)
{
	button_pressed = true;
	PORTA ->ISFR  = IFSR_LR_RET;
}

bool button_check()
{
	status = button_pressed;
	button_pressed = false;
	return status;
}


