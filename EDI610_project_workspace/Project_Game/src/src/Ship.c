/*
 * Ship.c
 *
 *  Created on: 23 apr. 2018
 *      Author: os2422el-s
 */

#include "ship.h"
#include <stdlib.h>

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

void Ship_place(Ship* self, int m[12][12]) {
	// Kollar att båten ligger innanför matrisen
	if (self->x < 11 && self->x >= 1 && self->y < 11 && self->y >= 1) {
		switch (self->dir) {
		case 0:
			//Kollar att alla platser båten kommer ligga på är 0:or
			for (int k = self->y; k > (self->y - self->seg); k--) {
				for (int i = k - 1; i <= k + 1; i++) {
					for (int j = self->x - 1; j <= self->x + 1; j++) {
						if (m[j][i] != 0) {
							return;
						}
					}

				}

			}
			//kollar att skeppet inte sträcker sig utanför matrisen
			if (self->y - self->seg >= 0) {
				for (int i = 0; i < (self->seg); i++) {

					m[self->y-i][self->x] = 1;
				}
			}
			break;
		case 1:
			for (int k = self->x; k < (self->x + self->seg); k++) {
				for (int i = self->y-1; i <= self->y+1; i++) {
					for (int j = k - 1; j <= k + 1; j++) {
						if (m[i][j] != 0) {
							return;
						}
					}

				}

			}
			if (self->x + self->seg <= 11) {
				for (int i = 0; i < (self->seg); i++) {
					m[self->y][self->x+i] = 1;
				}
			}
			break;
		case 2:
			for (int k = self->y; k < (self->y + self->seg); k++) {
				for (int i = k - 1; i <= k + 1; i++) {
					for (int j = self->x - 1; j <= self->x + 1; j++) {
						if (m[i][j] != 0) {
							return;
						}
					}

				}

			}
			if (self->y + self->seg <= 11) {
				for (int i = 0; i < (self->seg); i++) {
					m[self->y+i][self->x] = 1;
				}
			}
			break;
		case 3:
			for (int k = self->x; k > (self->x-self->seg); k--) {
				for (int i = self->y - 1; i <= self->y + 1; i++) {
					for (int j = k - 1; j <= k + 1; j++) {
						if (m[i][j] != 0) {
							return;
						}
					}

				}

			}
			if (self->x - self->seg >= 0) {
				for (int i = 0; i < (self->seg); i++) {
					m[self->y][self->x-i] = 1;
				}
			}
			break;
		}
	}
}
/** Markerar [x][y] och sedan ska man kunna styra med piltangenterna för att öka/minska x/y. trycka enter för att bomba och om
värdet är en 1:a i matrisplatsen ska värdet ändras till en 2:a, ifall hela båten är sprängd ska värdena ändras till 3:or*/
void Bomb(int m[12][12], int x, int y){
	if (m[y][x]==0){
		m[y][x]=2;
	}
	else if(m[y][x]==1){
		m[y][x]=3;
	}

}
