/*
 * vga.c
 *
 *  Created on: 6 mars 2017
 *      Author: eit-arj
 */

#include "vga.h"

void drawPixel(unsigned int x, unsigned int y, unsigned char color){
	if(color!=229){
		unsigned int pixelAddress = y * VGA_WIDTH + x;
			*VGA_BASE = (pixelAddress << 8) + color;
	}

}

void drawSquare(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, unsigned char color){
	int i, j;

	for (i = startY; i <= endY; i++) {
		for (j = startX; j <= endX; j++) {
			drawPixel(j, i, color);
		}
	}
}


/*
 * Draw a texture by using an array of colors (stored in textures.h) then
 * drawing them pixel-by-pixel
 */
void drawTexture(unsigned int startX, unsigned int startY, unsigned char textureColor[]){
	int i, j;
	unsigned char index = 0;
	for (i = startY; i <= startY+15; i++) {
			for (j = startX; j <= startX+15; j++) {
				drawPixel(j, i, textureColor[index]);
				index++;
			}
		}
}



void clearScreen(unsigned char bgColor){
	drawSquare(0,0,VGA_WIDTH-1,VGA_HEIGHT-1, bgColor);
}
