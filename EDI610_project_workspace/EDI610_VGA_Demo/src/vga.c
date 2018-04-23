/*
 * vga.c
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#include "vga.h"

void drawPixel(unsigned int x, unsigned int y, unsigned char color){
	unsigned int pixelAddress = y * VGA_WIDTH + x;
	*VGA_BASE = (pixelAddress << 8) + color;
}

void drawSquare(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, unsigned char color){
	int i, j;

	for (i = startY; i <= endY; i++) {
		for (j = startX; j <= endX; j++) {
			drawPixel(j, i, color);
		}
	}
}

void clearScreen(unsigned char bgColor){
	drawSquare(0,0,VGA_WIDTH-1,VGA_HEIGHT-1, bgColor);
}
