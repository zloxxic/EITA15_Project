/*
 * timers.h
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#define TIMER1_CTRL (unsigned int*) 0x41C10000
#define TIMER1_LOAD (unsigned int*) 0x41C10004
#define TIMER1_CR 	(unsigned int*) 0x41C10008

#define TIMER2_CTRL (unsigned int*) 0x41C00000
#define TIMER2_LOAD (unsigned int*) 0x41C00004
#define TIMER2_CR 	(unsigned int*) 0x41C00008

#endif /* SRC_TIMERS_H_ */
