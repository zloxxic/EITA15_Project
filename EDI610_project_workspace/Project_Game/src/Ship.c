/*
 * Ship.c
 *
 *  Created on: 23 apr. 2018
 *      Author: os2422el-s
 */

#include "ship.h"
#include <stdlib.h>


void Ship_init(Ship* self, int x, int y, int seg, int dir){
	self->x = x;
	self->y = y;
	self->seg = seg;
	self->dir = dir;
}

Ship* Ship_create(int x, int y, int seg, int dir){
	Ship* res = (Ship*) malloc(sizeof(Ship));
	Ship_init(res, x, y, seg, dir);
	return res;
}

void Ship_reset(Ship* self){
}

void Ship_destroy(Ship* ship){
	if(ship){
		Ship_reset(ship);
		free(ship);
	}
}

int Ship_x(Ship* self){
	return self-> x;
}
int Ship_y(Ship* self){
	return self-> y;
}
int Ship_seg(Ship* self){
	return self-> seg;
}
int Ship_dir(Ship* self){
	return self-> dir;
}
