/*
 * gpio.c includes all the definitions of functions in gpio.h
 * This include gpio_Init()
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#include "gpio.h"


void gpio_init()
{
	//****************Turning on Clock for GPIOs**************************
	SIM -> SCGC5 |= PORT_A;
	SIM -> SCGC5 |= PORT_B;

	//****************Configuring Pins as GPIO****************************
	PORTB -> PCR[RED] = (uint32_t)(0x00000100);
	PORTB -> PCR[GREEN] = (uint32_t)(0x00000100);
	PORTD -> PCR[BLUE]= (uint32_t)(0x00000100);

	//****************Configuring Pins as output**************************
	GPIOB -> PDDR |= red_LED;
	GPIOB -> PDDR |= green_LED;
	GPIOD -> PDDR |= blue_LED;

	//****************Initializing Pins to Low****************************
	GPIOB -> PDOR |= red_LED;
	GPIOB -> PDOR |= green_LED;
	GPIOD -> PDOR |= blue_LED;
}

void Init_TPM(uint16_t period)
{
	//Gating register to enable clock to TPM0
	SIM ->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
	SIM ->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM ->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	//Blue LED as PWM
	PORTD ->PCR[BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD ->PCR[BLUE] |= PORT_PCR_MUX(4);

	//Red LED as PWM
	PORTB ->PCR[RED] &= ~PORT_PCR_MUX_MASK;
	PORTB ->PCR[RED] |= PORT_PCR_MUX(3);

	//Green LED as PWM
	PORTB ->PCR[GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB ->PCR[GREEN] |= PORT_PCR_MUX(3);

	//Set Clock Source as 48 Mhz
	SIM -> SOPT2 |=(SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	//SIM -> SOPT4 |=(SIM_SOPT4_TPMSRC(1) | SIM_SOPT4_PLLFLLSEL_MASK);

	//Load the counter MOD
	(TPM0 ->MOD) = (TPM2 ->MOD)  = (period -1);
	// Set Prescaler as 2
	(TPM0 ->SC) = (TPM2 ->SC) = TPM_SC_PS(6);

	// Operation in Debug Mode
	TPM0 -> CONF |= TPM_CONF_DBGMODE(3);
	TPM2 -> CONF |= TPM_CONF_DBGMODE(3);

	// Set Channel 1 for PWM
	TPM0 -> CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2 -> CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2 -> CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	// Set Initial Duty Cycle
	TPM0 -> CONTROLS[1].CnV =0;
	TPM2 -> CONTROLS[0].CnV =0;
	TPM2 -> CONTROLS[1].CnV =0;

	//Start TPM

	TPM0 -> SC |= TPM_SC_CMOD(1);
	TPM2 -> SC |= TPM_SC_CMOD(1);

}
