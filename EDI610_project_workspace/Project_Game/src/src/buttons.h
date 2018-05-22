/*
 * buttons.h
 *
 *  Created on: 7 mars 2017
 *      Author: eit-arj
 */

#ifndef SRC_BUTTONS_H_
#define SRC_BUTTONS_H_

#define BUTTONS_DATA 	(unsigned int *) 0x40000000
#define BUTTONS_CONTROL (unsigned int *) 0x40000004
#define BUTTONS_GIER 	(unsigned int *) 0x4000011C
#define BUTTONS_ISR 	(unsigned int *) 0x40000120
#define BUTTONS_IER 	(unsigned int *) 0x40000128

#define UP 		0b00010
#define DOWN	0b10000
#define MIDDLE	0b00001
#define LEFT 	0b00100
#define RIGHT	0b01000

#endif /* SRC_BUTTONS_H_ */
