#include <stdio.h>
#include <stdlib.h>
#include "integral.h"
double pi = 3.14159265;

int main()
{
    printf("5x from 0 to 10 = %lf", dIntegral(f, 0, 10, defaultStep));
    printf("\n");
    printf("cosx from 0 to pi/2 = %lf", dIntegral(g, 0, pi/2, defaultStep));
    printf("\n");
    printf("sinx + cosx + lnx from 10 to 100 = %lf", dIntegral(h, 10, 100, defaultStep));
    return 0;
}
