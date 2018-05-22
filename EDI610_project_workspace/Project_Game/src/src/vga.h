/*
 * vga.h
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#ifndef SRC_VGA_H_
#define SRC_VGA_H_

#define VGA_BASE (unsigned int*) 0x44A10000

#define VGA_WIDTH 640
#define VGA_HEIGHT 480

#define COLOR_RED 	0b11100000
#define COLOR_GREEN 0b00011100
#define COLOR_BLUE 	0b00000011
#define COLOR_BLACK 0b00000000
#define COLOR_WHITE 0b11111111
#define COLOR_YELLOW 0b11111100

void drawPixel(unsigned int x, unsigned int y, unsigned char color);
void drawSquare(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, unsigned char color);
void drawTexture(unsigned int startX, unsigned int startY, unsigned char textureColor[]);
void clearScreen(unsigned char bgColor);

#endif /* SRC_VGA_H_ */
