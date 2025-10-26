#include "integral.h"
#include <math.h>
double dIntegral(double (*func)(double), double left, double right, double step) {
    double crn = left, result = 0;
    while (crn < right) {
        result += func(crn) + func(crn + step);
        crn += step;
    }
    result *= step / 2;
    return result;
}
double f(double x) { return 5 * x; }
double g(double x) { return cos(x); }
double h(double x) { return sin(x) + cos(x) + log(x); }
double defaultStep = 0.00001;