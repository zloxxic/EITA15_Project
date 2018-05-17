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
	Ship* ship = Ship_create(1, 10, 8, 0);
	Ship* ship1 = Ship_create(5, 3, 5, 2);
	Ship* ship3 = Ship_create(1, 1, 10, 1);
	Ship* ship2 = Ship_create(10, 3, 6, 2);
	Ship* ship4 = Ship_create(10,10,8,3);

	Ship_place(ship,net);
	Ship_place(ship1,net);
	Ship_place(ship3,net);
	Ship_place(ship2,net);
	Ship_place(ship4,net);

	for (i = 0; i < 12; i++) {

		for (j = 0; j < 12; j++) {
			if (net[i][j] == 0) {
				printf("x");
			} else {
				printf("B");
			}
		}
		printf("\n");
	}
	Ship_destroy(ship);
	Ship_destroy(ship1);
	Ship_destroy(ship2);
	Ship_destroy(ship3);
	Ship_destroy(ship4);
	return 0;
}

