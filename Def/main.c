#include <stdio.h>
#include <stdlib.h>
#include "integral.h"
double pi = 3.14159265;
struct func {
    char* description;
    double (*f)(double);
} menu[] = { {"5x", f}, {"cosx", g}, {"sinx+cosx+lnx", h} };
int main()
{
    int n;
    printf("Enter code of func\n");
    for (int i = 0; i < 3; ++i) {
        printf("%d. %s\n", i+1, menu[i].description);
    }
    double left, right, delta;
    scanf_s("%d", &n);
    n -= 1;
    printf("Enter limits and step\n");
    scanf_s("%lf%lf%lf", &left, &right, &delta);
    if (delta <= 0) delta = defaultStep;
    printf("%s from %lf to %lf = %lf", menu[n].description, left, right, dIntegral(menu[n].f, left, right, delta));
    return 0;
}
