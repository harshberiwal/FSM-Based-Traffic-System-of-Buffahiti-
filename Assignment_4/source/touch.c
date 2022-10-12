/*
 * touch.c includes all the definitions of functions declared in Touch.h
 * This include Touch_Init() and Touch_Poll() and its IRQHandler.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#include "touch.h"
#include "board.h"
#include "MKL25Z4.h"

#define TOUCH_OFFSET 	460
#define TOUCH_THRESHOLD 20
#define MAX_16 			(0xFFFF)

uint8_t touch_pressed = 0;

void Touch_Init()
{
    //***************Enable clock for TSI PortB 16 and 17***************************
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

    //Setting End of Scan Flag, RefChrg as 8uA, ExtChrg as 64uA
    //and 11 as the number of Electrodes
    TSI0->GENCS |= (TSI_GENCS_ESOR_MASK | TSI_GENCS_REFCHRG(OSC_PRESCALER)
    				| TSI_GENCS_EXTCHRG(ELEC_CHARGE) | TSI_GENCS_PS(OSC_PRESCALER)
					| TSI_GENCS_NSCN(ELECTRODES)     | TSI_GENCS_TSIIEN_MASK
                    | TSI_GENCS_STPE_MASK);

    //***************************Enabling the TSI Sensor****************************
    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;
}

bool Touch_Poll(void)
{
	int scanned_val;
    //************************Using channel 10 of The TSI***************************
    TSI0->DATA =  TSI_DATA_TSICH(CHANNEL);

    //***********************Triggering to scan the value***************************
    TSI0->DATA |= TSI_DATA_SWTS_MASK;

    //******************************Taking the Value********************************
    scanned_val = (uint16_t)((TSI0 -> DATA) & MAX_16);

    //*********************EOSF flag set after getting the value********************
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;

    if((scanned_val - TOUCH_OFFSET) > TOUCH_THRESHOLD)
    	return true;
    else
    	return false;
}




