/*
 * displays.h
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#ifndef SRC_DISPLAYS_H_
#define SRC_DISPLAYS_H_

#define SEVEN_SEGMENT_DISPLAY_BASE 0x44A00000

void displayDigitAtIndex(unsigned char index, unsigned char digit);
unsigned int numberOfDigits(unsigned int x);
void displayNumber(unsigned int x);
void resetDisplayAtIndex(unsigned char index);
void resetDisplays();

#endif /* SRC_DISPLAYS_H_ */
