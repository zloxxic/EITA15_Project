/*
 * displays.c
 *
 *  Created on: 7 mars 2017
 *      Author: eit-arj
 */

#include "displays.h"

const unsigned int display_codes[] = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111,
	0b0
};

void displayDigitAtIndex(unsigned char index, unsigned char digit) {
	if ((index < 8) && (digit < 10))
		*((unsigned int *) (SEVEN_SEGMENT_DISPLAY_BASE + index * 4)) =
				display_codes[digit];
}

unsigned int numberOfDigits(unsigned int x) {
	unsigned int count = 1;
	while (x / 10 > 0) {
		count++;
		x = x / 10;
	}
	return count;
}

void displayNumber(unsigned int x) {
	unsigned int index = 0;
	if (numberOfDigits(x) < 9)
		do {
			displayDigitAtIndex(index, x % 10);
			index++;
			x = x / 10;
		} while (x > 0);
}

void resetDisplayAtIndex(unsigned char index){
	displayDigitAtIndex(index, 10);
}

void resetDisplays(){
	unsigned char index;
	for(index = 0; index < 8; index++){
		resetDisplayAtIndex(index);
	}
}
