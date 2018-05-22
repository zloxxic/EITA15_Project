/*
 * Ship.c
 *
 *  Created on: 23 apr. 2018
 *      Author: os2422el-s
 */

#include "ship.h"
#include <stdlib.h>
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
						if (m[j][i] != 0) {
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
	srand(temp);
	while (nbr4 > 0) {
//		slump_x = random(9,47)+1;
//		//timeSinceStart /= 7;
//		temp = timeSinceStart;
//		slump_y = random(9,31)+1;
//		//timeSinceStart /= 7;
//		temp = timeSinceStart;
//		slump_dir = random(3,84);
		//srand((unsigned) time(&t));

		slump_x = (rand() % 10) + 1;
		slump_y = (rand() % 10) + 1;
		slump_dir = rand() % 4;
		Ship* tempShip = Ship_create(slump_x, slump_y, 4, slump_dir);
		char tempel = Ship_place(tempShip, m);
		if (tempel == 1) {
			nbr4--;
		} else {
			Ship_destroy(tempShip);
		}
		//timeSinceStart /= 7;
		temp = timeSinceStart;
	}
	while (nbr3 > 0) {
//		slump_x = random(9,37)+1;
//		//timeSinceStart /= 7;
//		slump_y = random(9,34)+1;
//		//timeSinceStart /= 7;
//		temp = timeSinceStart;
//		slump_dir = 1;
		//srand((unsigned) time(&t));

		slump_x = (rand() % 10) + 1;
		slump_y = (rand() % 10) + 1;
		slump_dir = rand() % 4;
		Ship* tempShip = Ship_create(slump_x, slump_y, 3, slump_dir);
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
//		slump_x = random(9,14)+1;
//		//timeSinceStart %= 7;
//		slump_y = random(9,5)+1;
//		//timeSinceStart %= 7;
//		slump_dir = random(3,23);
		//srand((unsigned) time(&t));

		slump_x = (rand() % 10) + 1;
		slump_y = (rand() % 10) + 1;
		slump_dir = rand() % 4;
		Ship* tempShip = Ship_create(slump_x, slump_y, 2, slump_dir);
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
void Bomb(int m[12][12], int x, int y) {
	if (m[y][x] == 0) {
		m[y][x] = 2;
	} else if (m[y][x] == 1) {
		m[y][x] = 3;
	}

}
