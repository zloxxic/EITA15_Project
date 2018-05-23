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
char Ship_place(Ship* self, int m[12][12]);
char Bomb(int m[12][12], int x, int y);
void placeBoard(int m[12][12]);
void checkIfBlown(int m[12][12], int y, int x);

int Ship_x(Ship* self);
int Ship_y(Ship* self);
int Ship_seg(Ship* self);
int Ship_dir(Ship* self);



#endif /* SRC_SHIP_H_ */
