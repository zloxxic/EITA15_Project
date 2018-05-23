/*
 * guessIndex.c

 *
 *  Created on: 23 maj 2018
 *      Author: vi8685pa-s
 */
#include "guessIndex.h"
#include "ship.h"
void guessIndex(int spelPlan[12][12]) {

	char elementsInList = 0;
	int x[100];
	int y[100];

	//int tempX= 0;
//	int tempY = 0;
	//bool
	//char foundHit = 0;

	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 11; j++) {

			//hittad träff
			if (spelPlan[i][j] == 3) {
			//	foundHit = 1;
			//	tempX = j;
			//	tempY = i;
				//bool
				char directions[] = { 0, 0, 0, 0 }; //höger,vänster,uppåt,neråt

				//kolla närliggande
				if (j + 1 < 11) {
					if (spelPlan[i][j + 1] == 3) {

						//träff höger
						directions[1] = 1;
						if (j + 2 < 11) {
							if (spelPlan[i][j + 2] == 0 || spelPlan[i][j + 2] == 1) {
								Bomb(spelPlan,j+2, i);
								return;
							}
//							else if (spelPlan[i][j + 2] == 3 && j - 2 >= 1) {
//								directions[0] = 1;
//								directions[1] = 0;
//
//								if (spelPlan[i][j - 2] == 0
//										|| spelPlan[i][j - 2] == 1) {
//									Bomb(spelPlan,j-2,i);
//									return;
//								}
//							}
						}
					}
				}
				//kolla närliggande
				if (j - 1 >= 1) {
					if (spelPlan[i][j - 1] == 3) {

						//träff vänster
						directions[0] = 1;
						if (j - 2 >= 1) {
							if (spelPlan[i][j - 2] == 0 || spelPlan[i][j - 2] == 1) {
								Bomb(spelPlan,j-2, i);
								return;
							}
//							else if (spelPlan[i][j - 2] == 3 && j + 2 < 11) {
//								directions[1] = 1;
//								directions[0] = 0;
//
//								if (spelPlan[i][j + 2] == 0
//										|| spelPlan[i][j + 2] == 1) {
//									Bomb(spelPlan,j, i + 2);
//									return;
//								}
//
//							}
						}
					}
				}
				//kolla närliggande
				if (i + 1 < 11) {
					if (spelPlan[i + 1][j] == 3) {

						//träff under
						directions[2] = 1;
						if (i + 2 < 11) {
							if (spelPlan[i + 2][j] == 0 || spelPlan[i + 2][j] == 1) {
								Bomb(spelPlan,j, i+2);
								return;
							}
//							else if (spelPlan[i + 2][j] == 3 && i - 2 >= 1) {
//								directions[3] = 1;
//								directions[2] = 0;
//
//								if (spelPlan[i + 2][j] == 0 || spelPlan[i + 2][j] == 1) {
//									Bomb(spelPlan, j, i+2);
//									return;
//								}
//							}
						}
					}
				}
				//kolla närliggande
				if (i - 1 >= 1) {
					if (spelPlan[i - 1][j] == 3) {

						//träff över
						directions[3] = 1;
						if (i - 2 >= 1) {
							if (spelPlan[i - 2][j] == 0 || spelPlan[i - 2][j] == 1) {
								Bomb(spelPlan,j, i - 2);
								return;
							}
//							else if (spelPlan[i + 2][j] == 3 && i - 2 >= 1) {
//								directions[2] = 1;
//								directions[3] = 0;
//								Bomb(spelPlan,j, i - 2);
//								return;
//							}
						}
					}
				}
				if (directions[0] == 0 && directions[1] == 0 && directions[2] == 0 && directions[3] == 0) {
					if (j + 1 < 11) {
						if (spelPlan[i][j + 1] == 0 || spelPlan[i][j + 1] == 1) {
							x[elementsInList] = j + 1;
							y[elementsInList] = i;
							elementsInList++;
							//Bomb(spelPlan,j+1, i);
						}
					}
					if (j - 1 >= 1) {
						if (spelPlan[i][j - 1] == 0 || spelPlan[i][j - 1] == 1) {
							x[elementsInList] = j-1;
							y[elementsInList] = i;
							elementsInList++;
							//Bomb(spelPlan,j-1, i);
						}
					}
					if (i + 1 < 11) {
						if (spelPlan[i + 1][j] == 0 || spelPlan[i + 1][j] == 1) {
							x[elementsInList] = j;
							y[elementsInList] = i+1;
							elementsInList++;
							//Bomb(spelPlan,j, i+1);
						}
					}
					if (i - 1 >= 1) {
						if (spelPlan[i - 1][j] == 0 || spelPlan[i - 1][j] == 1) {
							x[elementsInList] = j;
							y[elementsInList] = i-1;
							elementsInList++;
							//Bomb(spelPlan,j, i-1);
						}
					}
				}
			}
		}
	//}
	}
//	if(foundHit == 1){
//			Bomb(spelPlan,tempX, tempY-1);
//			Bomb(spelPlan,tempX, tempY+1);
//			Bomb(spelPlan,tempX+1, tempY);
//			Bomb(spelPlan,tempX-1, tempY);
//	}else{
	if(elementsInList == 0){
		for (int i = 1; i < 11; i++) {
				for (int j = 1; j < 11; j++) {
					if(spelPlan[i][j] == 0 || spelPlan[i][j] == 1){
						x[elementsInList] = j;
						y[elementsInList] = i;
						elementsInList++;
					}
				}
		}
	}
	int temp = random()%elementsInList;
		Bomb(spelPlan,x[temp],y[temp]);
		//Bomb(spelPlan, temp, temp);
		return;
	}


