/*
 * guessIndex.c

 *
 *  Created on: 23 maj 2018
 *      Author: vi8685pa-s
 */
#include "guessIndex.h"
#include "ship.h"
void guessIndex(int spelPlan[12][12]) {

	int elementsInList = 0;
	int x[100];
	int y[100];

	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 11; j++) {

			//hittad tr�ff
			if (spelPlan[i][j] == 3) {
				//bool
				char directions[] = { 0, 0, 0, 0 }; //h�ger,v�nster,upp�t,ner�t

				//kolla n�rliggande
				if (j + 1 < 11) {
					if (spelPlan[i][j + 1] == 3) {

						//tr�ff h�ger
						directions[1] = 1;
						if (j + 2 < 11) {
							if (spelPlan[i][j + 2] == 0 || spelPlan[i][j + 2] == 1) {
								Bomb(spelPlan,j+2, i);
								return;
							}
						}
					}
				}
				//kolla n�rliggande
				if (j - 1 >= 1) {
					if (spelPlan[i][j - 1] == 3) {

						//tr�ff v�nster
						directions[0] = 1;
						if (j - 2 >= 1) {
							if (spelPlan[i][j - 2] == 0 || spelPlan[i][j - 2] == 1) {
								Bomb(spelPlan,j-2, i);
								return;
							}
						}
					}
				}
				//kolla n�rliggande
				if (i + 1 < 11) {
					if (spelPlan[i + 1][j] == 3) {

						//tr�ff under
						directions[2] = 1;
						if (i + 2 < 11) {
							if (spelPlan[i + 2][j] == 0 || spelPlan[i + 2][j] == 1) {
								Bomb(spelPlan,j, i+2);
								return;
							}
						}
					}
				}
				//kolla n�rliggande
				if (i - 1 >= 1) {
					if (spelPlan[i - 1][j] == 3) {

						//tr�ff �ver
						directions[3] = 1;
						if (i - 2 >= 1) {
							if (spelPlan[i - 2][j] == 0 || spelPlan[i - 2][j] == 1) {
								Bomb(spelPlan,j, i - 2);
								return;
							}
						}
					}
				}
				if (directions[0] == 0 && directions[1] == 0 && directions[2] == 0 && directions[3] == 0) {
					if (j + 1 < 11) {
						if (spelPlan[i][j + 1] == 0 || spelPlan[i][j + 1] == 1) {
							x[elementsInList] = j + 1;
							y[elementsInList] = i;
							elementsInList++;
						}
					}
					if (j - 1 >= 1) {
						if (spelPlan[i][j - 1] == 0 || spelPlan[i][j - 1] == 1) {
							x[elementsInList] = j-1;
							y[elementsInList] = i;
							elementsInList++;
						}
					}
					if (i + 1 < 11) {
						if (spelPlan[i + 1][j] == 0 || spelPlan[i + 1][j] == 1) {
							x[elementsInList] = j;
							y[elementsInList] = i+1;
							elementsInList++;
						}
					}
					if (i - 1 >= 1) {
						if (spelPlan[i - 1][j] == 0 || spelPlan[i - 1][j] == 1) {
							x[elementsInList] = j;
							y[elementsInList] = i-1;
							elementsInList++;
						}
					}
				}
			}
		}
	}
	if(elementsInList == 0){
		for (int i = 1; i < 11; i++) {
				for (int j = 1; j < 11; j++) {
					if(spelPlan[i][j] == 0 || spelPlan[i][j] == 1){
						x[elementsInList] = j;
						y[elementsInList] = i;
						elementsInList++;
					}
					//f�r kraftfull f�r m�nskligheten
					//Bomb(spelPlan,j,i);
				}
		}
	}
	int temp = random()%elementsInList;
		Bomb(spelPlan,x[temp],y[temp]);
		return;
	}


