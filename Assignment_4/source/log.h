/*
 * log.h differentiates between DEBUG AND PRODUCTION CODE(DEBUG AND RELEASE)
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code
 */

#ifndef LOG_H_
#define LOG_H_

#define TRANSITION_TIME	16
#ifdef NDEBUG
#define STOP_TIME 		(320 + TRANSITION_TIME)
#define GO_TIME   		(320 + TRANSITION_TIME)
#define WARNING_TIME 	(80 + TRANSITION_TIME)
#define LOG(...)

#else
#define STOP_TIME 		(80 + TRANSITION_TIME)
#define GO_TIME   		(80 + TRANSITION_TIME)
#define WARNING_TIME 	(48 + TRANSITION_TIME)
#define LOG PRINTF
#endif

#endif
