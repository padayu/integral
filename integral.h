#include <stdio.h>
#include <math.h>
 
typedef double afunc(double);
 
double root(afunc *f, afunc *g, double a, double b, double eps1, afunc *f_, afunc *g_);
double integral(afunc *f, double a, double b, double eps2);
double test_f1(double);
double test_f2(double);
double test_f3(double);
double test_f1_(double);
double test_f2_(double);
double test_f3_(double);