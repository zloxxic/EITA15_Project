/*
 * interrupt_controller.h
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#ifndef SRC_INTERRUPT_CONTROLLER_H_
#define SRC_INTERRUPT_CONTROLLER_H_

#define IER 	(unsigned int*) 0x41200008
#define IAR 	(unsigned int*) 0x4120000C
#define MER 	(unsigned int*) 0x4120001C
#define IVAR0	(unsigned int*) 0x41200100
#define IVAR1	(unsigned int*) 0x41200104
#define IVAR2	(unsigned int*) 0x41200108
#define IVAR3	(unsigned int*) 0x4120010C
#define IVAR4	(unsigned int*) 0x41200110

#define BUTTONS_IRQ 	0
#define TIMER1_IRQ		1
#define TIMER2_IRQ		2
#define GPIO_IRQ		3
#define KEYBOARD_IRQ	4



#endif /* SRC_INTERRUPT_CONTROLLER_H_ */
