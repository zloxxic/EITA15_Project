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
	int net[12][12];
	int i, j;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 12; j++) {
			net[i][j] = 0;
		}
	}
	Ship* ship = Ship_create(5, 5, 3, 2);
	Ship* ship1 = Ship_create(5, 5, 4, 0);
	Ship* ship3 = Ship_create(2, 2, 4, 1);
	Ship* ship2 = Ship_create(7, 5, 2, 1);

	Ship_place(ship,net);
	Ship_place(ship1,net);
	Ship_place(ship3,net);
	Ship_place(ship2,net);

//	int nY = ship->y;
//	int nX = ship->x;
//	int nY1 = ship1->y;
//	int nX1 = ship1->x;
	//net[ship->Ship_x][ship->y] = 1;
	i = 0;
	j = 0;
	//kollar att x och y-värde ligger innanför matrisen, kanske inte behövs
//	if (nX < 10 && nX >= 0 && nY < 10 && nY >= 0) {
//		switch (ship->dir) {
//		case 0:
//
//			for (int k = 0; k < (ship->seg); k++) {
//				if (net[nY - k][nX] != 0) {
//					break;
//				}
//			}
//			//kollar att skeppet inte sträcker sig utanför matrisen
//			if (nY - ship->seg >= 0) {
//				for (i = 0; i < (ship->seg); i++) {
//
//					net[nY][nX] = 1;
//					nY--;
//					net[nX1][nY1] = 1;
//					nY1--;
//
//				}
//			}
//			break;
//		case 1:
//			for (int k = 0; k < (ship->seg); k++) {
//				if (net[nY][nX + k] != 0) {
//					break;
//				}
//			}
//			if (nX + ship->seg < 10) {
//				for (i = 0; i < (ship->seg); i++) {
//					net[nY][nX] = 1;
//					nX++;
//				}
//			}
//			break;
//		case 2:
//			for (int k = 0; k < (ship->seg); k++) {
//				if (net[(nY + k)][nX] != 0) {
//					break;
//				}
//			}
//			if (nY + ship->seg < 10) {
//				for (i = 0; i < (ship->seg); i++) {
//					net[nY][nX] = 1;
//					nY++;
//					net[nY1][nX1] = 1;
//					nY1++;
//				}
//			}
//			break;
//		case 3:
//			for (int k = 0; k < (ship->seg); k++) {
//				if (net[(nY)][nX - k] != 0) {
//					break;
//				}
//			}
//			if (nX - ship->seg >= 0) {
//				for (i = 0; i < (ship->seg); i++) {
//					net[nY][nX] = 1;
//					nX--;
//					net[nY1][nX1] = 1;
//					nX1--;
//				}
//			}
//			break;
//		}
//	}
	for (i = 0; i < 12; i++) {

		for (j = 0; j < 12; j++) {
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

