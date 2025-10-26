#ifndef integrator
#define integrator
double dIntegral(double (*func)(double), double left, double right, double step);
double f(double x);
double g(double x);
double h(double x);
double defaultStep;
#endif
