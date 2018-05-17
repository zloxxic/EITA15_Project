/*
 * ShipTest.c
 *
 *  Created on: 23 apr. 2018
 *      Author: os2422el-s
 */
#include <stdio.h>
#include "ship.h"
#include <stdlib.h>

int main(void) {
	int net[10][10];
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			net[i][j] = 0;
		}
	}
	Ship* ship = Ship_create(5, 5, 3, 2);
	Ship* ship1 = Ship_create(0, 0, 4, 2);
	int nY = ship->y;
	int nX = ship->x;
	int nY1 = ship1->y;
	int nX1 = ship1->x;
	int ch1, ch2;
	ch1 = getch();
	//net[ship->Ship_x][ship->y] = 1;
	i = 0;
	j = 0;
	//kollar att x och y-värde ligger innanför matrisen, kanske inte behövs
	if (nX < 10 && nX >= 0 && nY < 10 && nY >= 0) {
		switch (ship1->dir) {
		case 0:
			//kollar att skeppet inte sträcker sig utanför matrisen
			if (nY - ship->seg >= 0 && net[nX][nY]==0) {
				for (i = 0; i < (ship->seg); i++) {

					net[nY][nX] = 1;
					nY--;
					net[nX1][nY1] = 1;
					nY1--;

				}
			}
			break;
		case 1:
			if (nX + ship->seg < 10) {
				for (i = 0; i < (ship->seg); i++) {
					net[nY][nX] = 1;
					nX++;
				}
			}
			break;
		case 2:
			if (nY + ship->seg < 10) {
				for (i = 0; i < (ship->seg); i++) {
					net[nY][nX] = 1;
					nY++;
					net[nY1][nX1] = 1;
					nY1++;
				}
			}
			break;
		case 3:
			if (nX - ship->seg >= 0) {
				for (i = 0; i < (ship->seg); i++) {
					net[nY][nX] = 1;
					nX--;
					net[nY1][nX1] = 1;
					nX1--;
				}
			}
			break;
		}
	}
	for (i = 0; i < 10; i++) {

		for (j = 0; j < 10; j++) {
			if (net[i][j] == 0) {
				printf("a");
			} else {
				printf("b");
			}
		}
		printf("\n");
	}
	Ship_destroy(ship);
	Ship_destroy(ship1);
	return 0;
}

