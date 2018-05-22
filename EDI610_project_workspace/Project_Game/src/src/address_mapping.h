/*
 * address_mapping.h
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#ifndef SRC_ADDRESS_MAPPING_H_
#define SRC_ADDRESS_MAPPING_H_

/*
 * Switches registers
 */
#define SWITCHES_DATA 		(unsigned int*) 0x40020000
#define SWITCHES_CONTROL 	(unsigned int*) 0x40020004

/*
 * LED registers
 */
#define LED_DATA 			(unsigned int*) 0x40010000
#define LED_CONTROL 		(unsigned int*) 0x40010004

#endif /* SRC_ADDRESS_MAPPING_H_ */
