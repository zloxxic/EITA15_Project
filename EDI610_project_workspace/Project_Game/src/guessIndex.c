/*
 * guessIndex.c

 *
 *  Created on: 23 maj 2018
 *      Author: vi8685pa-s
 */
#include "guessIndex.h"
#include "ship.h"
void guessIndex(char spelPlan[12][12]) {

	char elementsInList = 0;
	char x[100];
	char y[100];

	//bool
	char foundHit = 0;

	for (char i = 1; i < 11; i++) {
		for (char j = 1; j < 11; j++) {

			//hittad träff
			if (spelPlan[i][j] == 3) {
				foundHit = 1;
				//bool
				char directions[] = { 0, 0, 0, 0 }; //höger,vänster,uppåt,neråt

				//kolla närliggande
				if (j + 1 < 11) {
					if (spelPlan[i][j + 1] == 3) {

						//träff höger
						directions[1] = true;
						if (j + 2 < 11) {
							if (spelPlan[i][j + 2] == 0
									|| spelPlan[i][j + 2] == 1) {
								bomb(i, j + 2);
								return;
							} else if (spelPlan[i][j + 2] == 3 && j - 2 >= 1) {
								directions[0] = true;
								directions[1] = false;

								if (spelplan[i][j - 2] == 0
										|| spelPlan[i][j - 2] == 1) {
									bomb(i, j - 2);
									return;
								}
							}
						}
					}
				}
				//kolla närliggande
				if (j - 1 >= 1) {
					if (spelPlan[i][j - 1] == 3) {

						//träff vänster
						directions[0] = true;
						if (j - 2 >= 1) {
							if (spelPlan[i][j - 2] == 0
									|| spelPlan[i][j - 2] == 1) {
								bomb(i, j - 2);
								return;
							} else if (spelPlan[i][j - 2] == 3 && j + 2 < 11) {
								directions[1] = true;
								directions[0] = false;

								if (spelplan[i][j + 2] == 0
										|| spelPlan[i][j + 2] == 1) {
									bomb(i, j + 2);
									return;
								}

							}
						}
					}
				}
				//kolla närliggande
				if (i + 1 < 11) {
					if (spelPlan[i + 1][j] == 3) {

						//träff under
						directions[2] = true;
						if (i + 2 < 11) {
							if (spelPlan[i + 2][j] == 0
									|| spelPlan[i + 2][j] == 1) {
								bomb(i + 2, j);
								return;
							} else if (spelPlan[i + 2][j] == 3 && i - 2 >= 1) {
								directions[3] = true;
								directions[2] = false;

								if (spelplan[i + 2][j] == 0
										|| spelPlan[i + 2][j] == 1) {
									bomb(i + 2, j);
									return;
								}
							}
						}
					}
				}
				//kolla närliggande
				if (i - 1 >= 1) {
					if (spelPlan[i - 1][j] == 3) {

						//träff över
						directions[3] = true;
						if (i - 2 >= 1) {
							if (spelPlan[i + 2][j] == 0
									|| spelPlan[i + 2][j] == 1) {
								bomb(i + 2, j);
								return;
							} else if (spelPlan[i + 2][j] == 3 && i - 2 >= 1) {
								directions[2] = true;
								directions[3] = false;
								bomb(i - 2, j);
								return;
							}
						}
					}
				}
				if (directions[0] == 0 && directions[1] == 0
						&& directions[2] == 0 && directions[3] == 0) {
					if (j + 1 < 11) {
						if (spelPlan[i][j + 1] == 0
								|| spelPlan[i][j + 1] == 1) {
							x[elementsInList] = i;
							y[elementsInList] = j + 1;
							elementsInList++;
						}
					}
					if (j - 1 >= 1) {
						if (spelPlan[i][j - 1] == 0
								|| spelPlan[i][j - 1] == 1) {
							x[elementsInList] = i;
							y[elementsInList] = j - 1;
							elementsInList++;
						}
					}
					if (i + 1 < 11) {
						if (spelPlan[i + 1][j] == 0
								|| spelPlan[i + 1][j] == 1) {
							x[elementsInList] = i + 1;
							y[elementsInList] = j;
							elementsInList++;
						}
					}
					if (i - 1 >= 1) {
						if (spelPlan[i - 1][j] == 0
								|| spelPlan[i - 1][j] == 1) {
							x[elementsInList] = i - 1;
							y[elementsInList] = j;
							elementsInList++;
						}
					}
				}
			}
		}
	}

}

