/*
 * Ship.h
 *
 *  Created on: 23 apr. 2018
 *      Author: os2422el-s
 */

#ifndef SRC_SHIP_H_
#define SRC_SHIP_H_

typedef struct Ship{
	int x;
	int y;
	int seg;
	int dir;
}Ship;

Ship* Ship_create(int x, int y, int seg, int dir);
void Ship_destroy(Ship* self);

int Ship_x(Ship* self);
int Ship_y(Ship* self);
int Ship_seg(Ship* self);
int Ship_dir(Ship* self);



#endif /* SRC_SHIP_H_ */
