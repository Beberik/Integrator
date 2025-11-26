#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "integral.h"
#include "gauss.h"
double pi = 3.14159265;
long int awakeTime = 0;
struct func {
    char* description;
    double (*f)(double);
} menu[] = { {"5x", f}, {"cosx", g}, {"sinx+cosx+lnx", h} };
void DoIntegralExample() {
    int n;
    printf("Enter code of func\n");
    for (int i = 0; i < 3; ++i) {
        printf("%d. %s\n", i + 1, menu[i].description);
    }
    double left, right, delta;
    scanf_s("%d", &n);
    n -= 1;
    printf("Enter limits and step\n");
    scanf_s("%lf%lf%lf", &left, &right, &delta);
    if (delta <= 0) delta = defaultStep;
    printf("%s from %lf to %lf = %lf", menu[n].description, left, right, dIntegral(menu[n].f, left, right, delta));
}
void DoGaussExample(long double **table, long int n, long int m) {
    PrintTable(table, n, m);
    NormalizeRow(table[0], m);
    NormalizeRow(table[1], m);
    PrintTable(table, n, m);
    SwapRows(&table[0], &table[1]);
    SwapColumns(table, 0, 1, n);
    PrintTable(table, n, m);
    DecreaseRow(table[1], table[0], m);
    NormalizeRow(table[1], m);
    PrintTable(table, n, m);
    SwapRows(&table[0], &table[1]);
    printf("%ld %ld", FindColumnIndex(table, n, 1, 0), FindColumnIndex(table, n, 3, 0));
}
//void DoSolutions(long double** table, long int n, long int m) {
//	long int unaddX = 0;
//	for (long int i = 0; i < m - 1; i++) {
//		if (FindColumnIndex(table, n, i, 0) == -1) unaddX++;
//	}
//	long int* unaddedXTable; unaddedXTable = (long int*)malloc(unaddX * sizeof(long int));
//	long int cnt = 0, usedTemps = m - 1 - n - unaddX;
//	if (usedTemps < 0) usedTemps = 0;
//	for (long int i = 0; i < m - 1; i++) {
//		if (FindColumnIndex(table, n, i, 0) == -1) { unaddedXTable[cnt] = i; cnt++; }
//	}
//	long double** solvesTable;
//	solvesTable = (long double**)malloc(n * sizeof(long double*));
//	for (long int i = 0; i < n; i++) {
//		solvesTable[i] = (long double*)malloc((usedTemps + 1) * sizeof(long double));
//	}
//	solvesTable[0][0] = table[n - 1][m - 1];
//	cnt = 0;
//	long int temp = m - 2;
//	while (cnt < usedTemps) {
//		if (!InLIntArray(unaddedXTable, unaddX, temp)) { solvesTable[0][cnt + 1] = -table[n - 1][temp]; cnt++; }
//		temp--;
//	}
//	//printf("used temps = %d, n = %d, m = %d, unaddX = %d\n", usedTemps, n, m, unaddX);
//	//printf("row %d equals ", n - 1);
//	//PrintRow(solvesTable[0], usedTemps + 1);
//	for (long int i = 1; i < n; i++) {
//		solvesTable[i][0] = table[n - 1 - i][m - 1];
//		//printf("free of row %d = %lf\n", n - 1 - i, solvesTable[i][0]);
//		cnt = 0;
//		for (long int j = m - 2; j >= 0; j--) {
//			if (!InLIntArray(unaddedXTable, unaddX, j)) {
//				if (cnt < usedTemps) solvesTable[i][cnt + 1] = -table[n - 1 - i][j];
//				else {
//					//printf("placing %d into %d\n", cnt - usedTemps, i);
//					//PrintRow(solvesTable[i], usedTemps + 1);
//					if (cnt - usedTemps == i) break;
//					for (long int k = 0; k < usedTemps + 1; k++) solvesTable[i][k] -= solvesTable[cnt - usedTemps][k] * table[n - 1 - i][j];
//				}
//				cnt++;
//			}
//		}
//		//printf("row %d equals ", n - 1 - i);
//		//PrintRow(solvesTable[n - 1 - i], usedTemps + 1);
//	}
//	cnt = 0, temp = usedTemps;
//	printf("%ld\n", clock() - awakeTime);
//	printf("System solutions:\n");
//	for (long int i = 0; i < m - 1; i++) {
//		printf("x%d = ", i + 1);
//		if (InLIntArray(unaddedXTable, unaddX, i)) { printf("t%d\n", cnt + usedTemps + 1); cnt++; }
//		else if (n - i - 1 + cnt >= 0) {
//			PrintAnswerRow(solvesTable[n - i - 1 + cnt], usedTemps + 1);
//		}
//		else {
//			printf("t%d\n", temp);
//			temp--;
//		}
//	}
//}

// 1000 - 5s
// 2000 - 41s
// 3000 - 141s
// 4000 - 334s
// 5000 - 
// 6000 - 1455s

int main()
{
    //DoIntegralExample();
    
    long int n, m;
    scanf_s("%ld%ld", &n, &m);
    printf("Estimated operating time: %.0lfs\n", 5 * pow((m / 1000), 3));
	awakeTime = clock();
    long double** table;
    table = (long double**)malloc(n * sizeof(long double*));
    for (long int i = 0; i < n; i++) table[i] = (long double*)malloc(m * sizeof(long double));
	GenerateRandomTable(table, n, m, 0, 1, 1, 1, 1);
    /*for (long int i = 0; i < n; i++) {
        for (long int j = 0; j < m; j++)
            scanf_s("%lf", &table[i][j]);
    }*/
    //DoGaussExample(table, n, m);
	//printf("Input:\n");
    //PrintTable(table, n, m);
    long int nonZeroEquations = SimplifySystem(table, n, m, 1, 1);
    short flag = 0;
    switch (nonZeroEquations) {
    case -2: 
        printf("System simplified\n"); 
        printf("System solves:\n");
        for (long int i = 1; i < m + 1; i++) printf("x%d=t%d\n", i, i);
        break;
    case -1: printf("System is inconsistent\n"); break;
    default: 
        flag = 1;
        break;
    }
    if (flag) {
		n = nonZeroEquations;
		PlaceSolutions(table, n, m, 1, awakeTime); // doesn't work in gauss.c, works in main.c. Why?
		//DoSolutions(table, n, m);
		
    }
    return 0;
}

