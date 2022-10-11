/*
 * log.h
 *
 *  Created on: Oct 11, 2022
 *      Author: Harsh
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

#endif /* LOG_H_ */
