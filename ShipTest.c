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
	Ship* ship = Ship_create(1, 10, 10, 0);
	Ship* ship1 = Ship_create(3, 1, 6, 1);
	Ship* ship2 = Ship_create(10, 1, 10, 2);
	Ship* ship3 = Ship_create(8,10,6,3);

	Ship_place(ship,net);
	Ship_place(ship1,net);
	Ship_place(ship2,net);
	Ship_place(ship3,net);

	Bomb(net,1,1);
	Bomb(net,1,2);
	Bomb(net,1,3);
	Bomb(net,1,4);
	Bomb(net,1,5);
	Bomb(net,1,6);
	Bomb(net,1,7);
	Bomb(net,1,8);
	Bomb(net,1,9);
	Bomb(net,1,10);
	Bomb(net,1,11);


	for (i = 0; i < 12; i++) {

		for (j = 0; j < 12; j++) {
			if (net[i][j] == 0) {
				printf("x");
			} else if(net[i][j]==1) {
				printf("B");
			}
			else{
				printf("X");
			}
		}
		printf("\n");
	}
	Ship_destroy(ship);
	Ship_destroy(ship1);
	Ship_destroy(ship2);
	Ship_destroy(ship3);
	return 0;
}

