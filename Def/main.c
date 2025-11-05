#include <stdio.h>
#include <stdlib.h>
#include "integral.h"
#include "gauss.h"
double pi = 3.14159265;
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
int main()
{
    //DoIntegralExample();
    
    long int n, m;
    scanf_s("%ld%ld", &n, &m);
    long double** table;
    table = (long double**)malloc(n * sizeof(long double*));
    for (long int i = 0; i < n; i++) table[i] = (long double*)malloc(m * sizeof(long double));
    for (long int i = 0; i < n; i++) {
        for (long int j = 0; j < m; j++)
            scanf_s("%lf", &table[i][j]);
    }
    //DoGaussExample(table, n, m);
    PrintTable(table, n, m);
    long int nonZeroEquations = SimplifySystem(table, n, m, 1);
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
        n = nonZeroEquations; // все строки с номером более - уравнения с нулями
        long int unaddX = 0, usedTemps = CheckNonZeroRow(table[n-1], m-1) - 1;
        for (long int i = 0; i < m - 1; i++) {
            if (FindColumnIndex(table, n, i, 0) == -1) unaddX++;
        }
        long int* unaddedXTable; unaddedXTable = (long int*)malloc(unaddX * sizeof(long int));
        long int cnt = 0;
        for (long int i = 0; i < m - 1; i++) {
            if (FindColumnIndex(table, n, i, 0) == -1) { unaddedXTable[cnt] = i; cnt++; }
        }
        long double** solvesTable;
        solvesTable = (long double**)malloc(n * sizeof(long double*));
        for (long int i = 0; i < n; i++) {
            solvesTable[i] = (long double*)malloc((usedTemps + 1) * sizeof(long double));
        }
        solvesTable[0][0] = table[n - 1][m - 1];
        cnt = 1;
        for (long int i = 1; i < m - 1; i++) {
            if (!EqualAccurate(table[n - 1][m - 1 - i], 0)) {
                if (cnt < usedTemps + 1) {
                    solvesTable[0][cnt] = -table[n - 1][m - 1 - i];
                    cnt++;
                }
            }
        }
        //printf("row %d equals ", n - 1);
        //PrintRow(solvesTable[0], usedTemps + 1);
        long int offset;
        for (long int i = 1; i < n; i++) {
            solvesTable[i][0] = table[n - 1 - i][m - 1];
            //printf("free of row %d = %lf\n", n - 1 - i, solvesTable[i][0]);
            cnt = 1; offset = 0;
            for (long int j = 1; j < m - 1 - FindRowIndex(table[n - 1 - i], m); j++) {
                //printf("j = %d, free = %lf\n", j, solvesTable[i][0]);
                if (!EqualAccurate(table[n - 1 - i][m - 1 - j], 0)) {
                    // подстановка первичных параметров
                    if (cnt < usedTemps) {
                        //printf("Placing used temps, cnt = %d", cnt);
                        solvesTable[i][cnt] = -table[n - 1 - i][m - 1 - i];
                        
                        
                    }
                    // подстановка известных х
                    else {
                        //printf("Placing x, cnt = %d\n", cnt);
                        while (InLIntArray(unaddedXTable, unaddX, m - 1 - j - offset) && m - 1 - j - offset > -1) offset++;
                        //printf("offset = %d\n", offset);
                        if (m - 1 - j - offset == -1) offset = m - 1 - j; //offset = 0; //
                        for (long int k = 0; k < usedTemps + 1; k++) {
                            solvesTable[i][k] -= table[n - 1 - i][m - 1 - j - offset] * solvesTable[cnt - usedTemps - 1][k];
                        }
                    }
                    cnt++;
                }
            }
            //printf("row %d = ", n - i - 1);
            //PrintRow(solvesTable[i], usedTemps + 1);
        }
        cnt = 0;
        printf("System solutions:\n");
        for (long int i = 0; i < m-1; i++) {
            printf("x%d = ", i+1);
            if (InLIntArray(unaddedXTable, unaddX, i)) { printf("t%d\n", cnt+1); cnt++; }
            else printf("%lf\n", solvesTable[n-i-1+cnt][0]);
        }
        
        //PrintTable(solvesTable, n, usedTemps + 1);
        /*printf("System solves:\n");
        cnt = 0;
        for (long int i = 0; i < m - 1; m++) {
            printf("x%d");
        }*/
    }
    return 0;
}

