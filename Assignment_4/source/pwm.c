/*
 * pwm.c includes all the definitions of functions in pwm.h
 * This include Init_TPM() and initializes all the peripherals used
 * for transitions and PWM in the State_Machine.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * The Init_TPM code was inspired by the Alexander, Dean. Embedded Systems
 * Fundamentals with ARM Cortex-M Based Microcontrollers : A Practical Approach,
 * ARM Education Media, 2017. ProQuest Ebook Central.
 *
 * The fragments were taken from the Init_Blue_LED_TPM code(CH. 7) and was altered
 * accordingly for the application. Different Prescaler and Clock were used along with
 * initializing different Gated Registers and setting their bits for Port B.
 */

#include <PWM.h>

#define FIRST_CHANNEL   0
#define SECOND_CHANNEL  1
#define RESET 			0
#define MODE_LPTPM_INC	1
#define DEBUG_CONT		3
#define DIVIDE_BY_64 	6
#define TPM0_SET		4
#define TPM2_SET		3

void Init_TPM(uint16_t period)
{
	//****************Gating register to enable clock to TPM0**********************
	SIM ->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
	SIM ->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM ->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	//*****************************Blue LED as PWM*********************************
	PORTD ->PCR[BLUE] &= ~PORT_PCR_MUX_MASK;
	PORTD ->PCR[BLUE] |= PORT_PCR_MUX(TPM0_SET);

	//******************************Red LED as PWM*********************************
	PORTB ->PCR[RED] &= ~PORT_PCR_MUX_MASK;
	PORTB ->PCR[RED] |=  PORT_PCR_MUX(TPM2_SET);

	//*****************************Green LED as PWM********************************
	PORTB ->PCR[GREEN] &= ~PORT_PCR_MUX_MASK;
	PORTB ->PCR[GREEN] |= PORT_PCR_MUX(TPM2_SET);

	//**********************Set Clock Source as 48 Mhz*****************************
	SIM -> SOPT2 |=(SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	//*************************Load the counter MOD********************************
	(TPM0 ->MOD) = (TPM2 ->MOD)  = (period -1);

	//**************************Set Prescaler as 6*********************************
	(TPM0 ->SC) = (TPM2 ->SC) = TPM_SC_PS(DIVIDE_BY_64);

	//************************Operation in Debug Mode******************************
	TPM0 -> CONF |= TPM_CONF_DBGMODE(DEBUG_CONT);
	TPM2 -> CONF |= TPM_CONF_DBGMODE(DEBUG_CONT);

	//************************Set Channel 1 for PWM********************************
	TPM0 -> CONTROLS[SECOND_CHANNEL].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2 -> CONTROLS[FIRST_CHANNEL].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2 -> CONTROLS[SECOND_CHANNEL].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	//************************Set Initial Duty Cycle*******************************
	TPM0 -> CONTROLS[SECOND_CHANNEL].CnV = RESET;
	TPM2 -> CONTROLS[FIRST_CHANNEL].CnV  = RESET;
	TPM2 -> CONTROLS[SECOND_CHANNEL].CnV = RESET;

	//******************************Start TPM**************************************
	TPM0 -> SC |= TPM_SC_CMOD(MODE_LPTPM_INC);
	TPM2 -> SC |= TPM_SC_CMOD(MODE_LPTPM_INC);
}


