/*
 * Ship.c
 *
 *  Created on: 23 apr. 2018
 *      Author: os2422el-s
 */

#include "ship.h"
#include <stdlib.h>
#include <stdio.h>
#include "displays.h"
extern timeSinceStart;

// Initializes what attributes a "Ship" has//
void Ship_init(Ship* self, int x, int y, int seg, int dir) {
	self->x = x;
	self->y = y;
	self->seg = seg;
	self->dir = dir;
}

// Creates an pointer which kind of works like an object
Ship* Ship_create(int x, int y, int seg, int dir) {
	Ship* res = (Ship*) malloc(sizeof(Ship));
	Ship_init(res, x, y, seg, dir);
	return res;
}

void Ship_reset(Ship* self) {
}

//Garbage collector
void Ship_destroy(Ship* ship) {
	if (ship) {
		Ship_reset(ship);
		free(ship);
	}
}

// Returns the ship self's start x coordinate
int Ship_x(Ship* self) {
	return self->x;
}

// Returns the ship self's start y coordinate
int Ship_y(Ship* self) {
	return self->y;
}

// Returns the length of ship self
int Ship_seg(Ship* self) {
	return self->seg;
}

/** Returns the direction of the ship, where
 * 0 is UP
 * 1 is RIGHT
 * 2 is DOWN
 * 3 is LEFT
 */
int Ship_dir(Ship* self) {
	return self->dir;
}

// Places a ship in the net m
char Ship_place(Ship* self, int m[12][12]) {

	// Checks that the boats start position is within the net
	if (self->x < 11 && self->x >= 1 && self->y < 11 && self->y >= 1) {

		// Different cases for each direction
		switch (self->dir) {

		// Up
		case 0:

			/** Checks where the boat is placed that it isn't going to collide with other boats, and that
			 * each spot is only going to have water as a neighbour, one of the terms of the game
			 * same for every direction
			 */
			for (int k = self->y; k > (self->y - self->seg); k--) {
				for (int i = k - 1; i <= k + 1; i++) {
					for (int j = self->x - 1; j <= self->x + 1; j++) {
						if (m[i][j] != 0) {
							return 0;
						}
					}

				}

			}
			/** Checks that the boat isn't stretching out of the net, and if it is not, the values of the net
			 *  changes at where the boat is placed.
			 *  Same for every direction
			 */
			if (self->y - self->seg >= 0) {
				for (int i = 0; i < (self->seg); i++) {

					m[self->y - i][self->x] = 1;
				}
				return 1;
			}
			break;
		case 1:
			for (int k = self->x; k < (self->x + self->seg); k++) {
				for (int i = self->y - 1; i <= self->y + 1; i++) {
					for (int j = k - 1; j <= k + 1; j++) {
						if (m[i][j] != 0) {
							return 0;
						}
					}

				}

			}
			if (self->x + self->seg <= 11) {
				for (int i = 0; i < (self->seg); i++) {
					m[self->y][self->x + i] = 1;
				}
				return 1;
			}
			break;
		case 2:
			for (int k = self->y; k < (self->y + self->seg); k++) {
				for (int i = k - 1; i <= k + 1; i++) {
					for (int j = self->x - 1; j <= self->x + 1; j++) {
						if (m[i][j] != 0) {
							return 0;
						}
					}

				}

			}
			if (self->y + self->seg <= 11) {
				for (int i = 0; i < (self->seg); i++) {
					m[self->y + i][self->x] = 1;
				}
				return 1;
			}
			break;
		case 3:
			for (int k = self->x; k > (self->x - self->seg); k--) {
				for (int i = self->y - 1; i <= self->y + 1; i++) {
					for (int j = k - 1; j <= k + 1; j++) {
						if (m[i][j] != 0) {
							return 0;
						}
					}

				}

			}
			if (self->x - self->seg >= 0) {
				for (int i = 0; i < (self->seg); i++) {
					m[self->y][self->x - i] = 1;
				}
				return 1;
			}
			break;
		}
	}
	return 0;
}

// Fills the net m with boats. Nbr4 is a four-length ship, nbr3 is a three-length ship etc.
void placeBoard(int m[12][12]) {

	// Amount of boats of each length between two and four
	int nbr4 = 1;
	int nbr3 = 2;
	int nbr2 = 3;
	int slump_x, slump_y, slump_dir;

	// Tries to place a four-length ship until it works.
	while (nbr4 > 0) {
		slump_x = (random()%10)+1;
		slump_y = ((random()/10)%10)+1;
		slump_dir = random()%4;

		Ship* tempShip = Ship_create(slump_x, slump_y, 4, slump_dir);

		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr4--;
			Ship_destroy(tempShip);
		} else {
			Ship_destroy(tempShip);
		}
	}

	// Tries to place two three-length boats until it works
	while (nbr3 > 0) {
		slump_x = (random()%10)+1;
		slump_y = ((random()/10)%10)+1;
		slump_dir = random()%4;

		Ship* tempShip = Ship_create(slump_x, slump_y, 3, slump_dir);
		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr3--;
			Ship_destroy(tempShip);
		} else {
			Ship_destroy(tempShip);
		}
	}

	// Tries to place three two-length boats until  it works
	while (nbr2 > 0) {
		slump_x = (random()%10)+1;
		slump_y = ((random()/10)%10)+1;
		slump_dir = random()%4;

		Ship* tempShip = Ship_create(slump_x, slump_y, 2, slump_dir);
		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr2--;
			Ship_destroy(tempShip);
		} else {
			Ship_destroy(tempShip);
		}
	}
}

/** Bombs the net m in the coordinates with the coordinates x and y m[y][x] changes value depending on what it is
 * 0 is water and changes to 2 for a miss
 * 1 is boat and changes to 3 for a hit
 * uses checkIfBlown to change values for a full hit boat to 4's in the net
 * if user or computer tries to bomb an already bombed spot it returns 0, else 1
 */
char Bomb(int m[12][12], int x, int y) {
	if (m[y][x] == 0) {
		m[y][x] = 2;
		return 1;
	} else if (m[y][x] == 1) {
		m[y][x] = 3;
		checkIfBlown(m, x, y);
		return 1;
	} else {
		return 0;
	}
	return 0;
}

/** Checks if an entire ship is hit, changes the values for the sunken boat in the net m
 * Is being used in method Bomb
 */
void checkIfBlown(int m[12][12], int y, int x){
	int upp = 0;
	int ner = 0;
	int va = 0;
	int ho = 0;
	int i,j;
	for(i = x-1; i >= x-4; i--){
		if(m[i][y] == 0 || m[i][y] == 2){
			break;
		}else if(m[i][y] == 1){
			return;
		}else{
			va--;
		}
	}

	for(i = x+1; i <= x+4; i++){
		if(m[i][y] == 0 || m[i][y] == 2){
			break;
		}else if(m[i][y] == 1){
			return;
		}else{
			ho++;
		}
	}
	for(i = y-1; i >= y-4; i--){
		if(m[x][i] == 0 || m[x][i] == 2){
			break;
		}else if(m[x][i] == 1){
			return;
		}else{
			upp--;
		}
	}
	for(i = y+1; i <= y+4; i++){
		if(m[x][i] == 0 || m[x][i] == 2){
			break;
		}else if(m[x][i] == 1){
			return;
		}else{
			ner++;
		}
	}
	if(va != 0 || ho != 0){
		for(j = x+va; j <= x+ho; j++){
			m[j][y] = 4;
		}
	}else{
		for(j = y+upp; j <= y+ner; j++){
			m[x][j] = 4;
		}
	}
}
