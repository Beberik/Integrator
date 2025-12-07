#include <math.h>
double accuracy = 0.0000001;
double e = 2.7182818;
long double Clamp(long double value, long double min, long double max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
// приблизительно сравнивает два значения
short EqualAccurate(long double a, long double b) {
	if (a > b) return a - b < accuracy; else return b - a < accuracy;
}
long double GaussianRandomValue(double x) {
	return pow(e, -(4 * x - 2) * (4 * x - 2)) - 0.01;
}
long double Abs(long double a) {
	if (a > 0) return a; return -a;
}