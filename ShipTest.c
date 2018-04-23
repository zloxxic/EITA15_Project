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
	char randomNamn;
	int net[10][10];
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			net[i][j] = 0;
		}
	}
	Ship* ship = Ship_create(5, 5, 3, 2);
	int nY = ship->y;
	int nX = ship->x;
	//net[ship->Ship_x][ship->y] = 1;
	i = 0;
	j = 0;
	switch (ship->dir) {
	case 0:

		for (i = 0; i < (ship->seg); i++) {
			net[ship->x][nY] = 1;
			nY--;
		}
		break;
	case 1:

		for (i = 0; i < (ship->seg); i++) {
			net[nX][ship->y] = 1;
			nX++;
		}
		break;
	case 2:
		for (i = 0; i < (ship->seg); i++) {
			net[ship->x][nY] = 1;
			nY++;
		}
		break;
	case 3:
		for (i = 0; i < (ship->seg); i++) {
			net[nX][ship->y] = 1;
			nX--;
		}
		break;
	}
	for(i = 0; i < 10; i++){

		for(j = 0; j < 10; j++){
			if(net[i][j]==0){
				printf("a");
			}else{
				printf("b");
			}
		}
		printf("\n");
	}
	Ship_destroy(ship);
	return 0;
}

