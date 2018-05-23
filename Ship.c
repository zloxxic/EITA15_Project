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


void Ship_init(Ship* self, int x, int y, int seg, int dir) {
	self->x = x;
	self->y = y;
	self->seg = seg;
	self->dir = dir;
}

Ship* Ship_create(int x, int y, int seg, int dir) {
	Ship* res = (Ship*) malloc(sizeof(Ship));
	Ship_init(res, x, y, seg, dir);
	return res;
}

void Ship_reset(Ship* self) {
}

void Ship_destroy(Ship* ship) {
	if (ship) {
		Ship_reset(ship);
		free(ship);
	}
}

int Ship_x(Ship* self) {
	return self->x;
}
int Ship_y(Ship* self) {
	return self->y;
}
int Ship_seg(Ship* self) {
	return self->seg;
}
int Ship_dir(Ship* self) {
	return self->dir;
}

char Ship_place(Ship* self, int m[12][12]) {
	// Kollar att båten ligger innanför matrisen
	if (self->x < 11 && self->x >= 1 && self->y < 11 && self->y >= 1) {
		switch (self->dir) {
		case 0:
			//Kollar att alla platser båten kommer ligga på är 0:or
			for (int k = self->y; k > (self->y - self->seg); k--) {
				for (int i = k - 1; i <= k + 1; i++) {
					for (int j = self->x - 1; j <= self->x + 1; j++) {
						if (m[i][j] != 0) {
							return 0;
						}
					}

				}

			}
			//kollar att skeppet inte sträcker sig utanför matrisen
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
void placeBoard(int m[12][12]) {
	int nbr4 = 1;
	int nbr3 = 2;
	int nbr2 = 3;
	int slump_x, slump_y, slump_dir;
	int temp = timeSinceStart;
	while (nbr4 > 0) {
		slump_x = (random()%10)+1;
		slump_y = ((random()/10)%10)+1;
		slump_dir = random()%4;

		Ship* tempShip = Ship_create(slump_x, slump_y, 4, slump_dir);
//		displayDigitAtIndex(0, tempShip->x);
//		displayDigitAtIndex(1, tempShip->y);
		displayDigitAtIndex(0, tempShip->dir);

		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr4--;
			Ship_destroy(tempShip);
		} else {
			Ship_destroy(tempShip);
		}
		//timeSinceStart /= 7;
		temp = timeSinceStart;
	}
	while (nbr3 > 0) {
		slump_x = (random()%10)+1;
		slump_y = ((random()/10)%10)+1;
		slump_dir = random()%4;

		Ship* tempShip = Ship_create(slump_x, slump_y, 3, slump_dir);
		displayDigitAtIndex(1, tempShip->dir);
		displayDigitAtIndex(2, tempShip->x);
		displayDigitAtIndex(3, tempShip->y);
		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr3--;
		} else {
			Ship_destroy(tempShip);
		}
		//timeSinceStart /= 7;
		temp = timeSinceStart;
	}
	while (nbr2 > 0) {
		slump_x = (random()%10)+1;
		slump_y = ((random()/10)%10)+1;
		slump_dir = random()%4;

		Ship* tempShip = Ship_create(slump_x, slump_y, 2, slump_dir);
		displayDigitAtIndex(4, tempShip->dir);
		displayDigitAtIndex(5, tempShip->x);
		displayDigitAtIndex(6, tempShip->y);
		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr2--;
		} else {
			Ship_destroy(tempShip);
		}
		//timeSinceStart %= 7;
		temp = timeSinceStart;
	}
}
/** Markerar [x][y] och sedan ska man kunna styra med piltangenterna för att öka/minska x/y. trycka enter för att bomba och om
 värdet är en 1:a i matrisplatsen ska värdet ändras till en 2:a, ifall hela båten är sprängd ska värdena ändras till 3:or*/
char Bomb(int m[12][12], int x, int y) {
	if (m[y][x] == 0) {
		m[y][x] = 2;
		return 1;
	} else if (m[y][x] == 1) {
		m[y][x] = 3;
		checkIfBlown(m, x, y);
		return 1;
	} else if (m[y][x]==3){
		return 0;
	}
	return 0;
}
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
