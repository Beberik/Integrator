#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "integral.h"
#include "gauss.h"
#include "mathFunctions.h"
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

// 1000 - 5s
// 2000 - 41s
// 3000 - 141s
// 4000 - 334s
// 5000 - 660s
// 6000 - 1455s

void DoDefaultGaussSolution(long double** table, long int n, long int m) {
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
        PlaceSolutions(table, n, m, 1, awakeTime);

    }
}
void DoAccuracyCheck(long double** table, long int n, long int m) {
    if (n != m - 1) printf("Wrong system size: n != m - 1");
    else {
        long double* fakeSolutions;
        fakeSolutions = (long double*)malloc(n * sizeof(long double));
        srand((getpid() << 16) + time(NULL));
        for (long int i = 0; i < n; i++) {
            long double rnd = 1;
            for (int i = 0; i < 2; i++) rnd *= (long double)rand() / RAND_MAX;
            fakeSolutions[i] = pow(rnd, 0.5);
        }
        for (long int i = 0; i < n; i++) {
            long double freep = 0;
            for (long int j = 0; j < n; j++) freep += table[i][j] * fakeSolutions[j];
            table[i][n] = freep;
        }
        
        SimplifySystem(table, n, m, 1, 1);
        long double** realSolutions = PlaceSolutions(table, n, m, 1, awakeTime);
        //PrintRow(fakeSolutions, n);
        long double arithmeticMean = 0, squareMean = 0;
        for (long int i = 0; i < n; i++) {
            arithmeticMean += Abs(fakeSolutions[i] - realSolutions[i][0]);
            squareMean += pow(fakeSolutions[i] - realSolutions[i][0], 2);
        }
        arithmeticMean /= n;
        squareMean /= n;
        squareMean = pow(squareMean, 0.5);
        printf("Arithmetic mean accuracy = %lf, square mean accuracy = %lf\n", arithmeticMean, squareMean);
    }
    // 1000 - 0.25 0.31
    // 2000 - 0.27 0.33
    // 3000 - 0.26 0.32
    // 4000 - 0.27 0.32
    // 5000 - 0.27 0.33
    // Arithmetic mean - 0.26 0.32
    
}
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
	GenerateRandomTable(table, n, m, 0, 1, 1, 1, 1, 3);
    /*for (long int i = 0; i < n; i++) {
        for (long int j = 0; j < m; j++)
            scanf_s("%lf", &table[i][j]);
    }
    DoGaussExample(table, n, m);
	printf("Input:\n");
    PrintTable(table, n, m);*/
    int* cnt;
    cnt = (int*)malloc(10 * sizeof(int));
    for (long int i = 0; i < 10; i++) cnt[i] = 0;
    for (long int i = 0; i < n; i++) {
        for (long int j = 0; j < n; j++) {
            cnt[(int)floor((table[i][j]) / 0.1)]++;
        }
    }
    printf("Distribution:\n"); // if values vary from 0 to 1, shows amount of values on 0-0.1, 0.1-0.2 ... 0.9-1
    for (long int i = 0; i < 10; i++) printf("%d ", cnt[i]);
    printf("\n");
    //DoDefaultGaussSolution(table, n, m);
    DoAccuracyCheck(table, n, m);
    return 0;
}
