#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <unistd.h>
#include "integral.h"
 
typedef double afunc(double);
 
extern afunc f;
extern afunc g;
extern afunc h;
extern afunc f_;
extern afunc g_;
extern afunc h_;
 
extern char* optarg; // option argument
 
int iterations = 0; // number of iterations for root function
 
double test_f1(double x) {
    return 0.1231 * x + 3;
}
 
double test_f2(double x) {
    return x * x * 2.33333;
}
 
double test_f3(double x) {
    return -4.42 * x + x * x;
}
 
double test_f1_(double x) {
    return 0.1231;
}
 
double test_f2_(double x) {
    return 4.66666 * x;
}
 
double test_f3_(double x) {
    return -4.42 + 2 * x;
}
 
afunc* test_functions[3] = {test_f1, test_f2, test_f3};
afunc* test_derivatives[3] = {test_f1_, test_f2_, test_f3_};
 
// Newton
// f_ = f', g_ = g'
double root(afunc *f, afunc *g, double a, double b, double eps1, afunc *f_, afunc *g_) {
    int sign1, sign; // sign1 = sgn(F'), sign = sgn(F'F'')
 
    if (f(a) - g(a) < 0) {
        sign1 = 1; // F'(x) > 0
    } else {
        sign1 = -1; // F'(x) <= 0
    }
 
    if (f((a + b) / 2) - g((a + b) / 2) >= (f(a) - g(a) + f(b) - g(b)) / 2) {
        if (sign1 == -1) {
            sign = 1; // F'(x)F''(x) >= 0
        } else {
            sign = -1; // F'(x)F''(x) < 0
        }
    } else {
        if (sign1 == 1) {
            sign = 1; // F'(x)F''(x) >= 0
        } else {
            sign = -1; // F'(x)F''(x) < 0
        }
    }
 
    double c; // intersection of the tangent with OX
    if (sign == 1) {
        c = b - (f(b) - g(b)) / (f_(b) - g_(b)); // from the right
        iterations++;
        while ((f(c) - g(c)) * (f(c - eps1) - g(c - eps1)) > 0) {
        c = c - (f(c) - g(c)) / (f_(c) - g_(c)); // more precision
        iterations++;
    }
    } else {
        c = a - (f(a) - g(a)) / (f_(a) - g_(a)); // from the left
        iterations++;
        while ((f(c) - g(c)) * (f(c + eps1) - g(c + eps1)) > 0) {
        c = c - (f(c) - g(c)) / (f_(c) - g_(c)); // more precision
        iterations++;
    }
    }
 
    return c;
}
 
// Rectangles
double integral(afunc *f, double a, double b, double eps2) {
    int n = 2; // initial value
    double h = (b - a) / n;
 
    double result1 = 0; // sum of areas
 
    for (int i = 0; i < n; i++) {
        result1 += f(a + (i + 0.5) * h); // adding each height
    }
    result1 *= h; // multiplying by width
 
    n *= 2; // twice as precisely
    double result2 = 0; // sum of areas
    h = (b - a) / n;
 
    for (int i = 0; i < n; i++) {
        result2 += f(a + (i + 0.5) * h); // adding each height
    }
    result2 *= h; // multiplying by width
 
    // p|In - I2n|
    double diff = (result1 - result2) / 3;
    if (diff < 0) {
        diff = -diff;
    }
 
    while (diff >= eps2) {
        result1 = result2;
        result2 = 0;
        n *= 2; // more precision
        h = (b - a) / n;
        for (int i = 0; i < n; i++) {
            result2 += f(a + (i + 0.5) * h); // adding each height
        }
        result2 *= h; // multiplying by width
 
        // updating diff
        diff = (result1 - result2) / 3;
        if (diff < 0) {
            diff = -diff;
        }
    }
 
    return result2;
}
 
int main(int argc, char** argv) {
 
    int c;
    int last_opt_ind;
 
    static struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"root", 0, 0, 'r'},
        {"iterations", 0, 0, 'i'},
        {"test-root", 1, 0, 'R'},
        {"test-integral", 1, 0, 'I'}
    };
 
    const char* short_options = "hriR:I:";
 
    c = getopt_long(argc, argv, short_options, long_options, &last_opt_ind); // get option
 
    if (c == -1) { // calculate the area
        double x_fg, x_fh, x_gh;
        x_fg = root(f, g, 1, 2, 0.00004, f_, g_);
        x_fh = root(f, h, -3, -2, 0.00004, f_, h_);
        x_gh = root(g, h, 0.6, 1, 0.00004, g_, h_);
 
        double integral1, integral2;
 
        // roots are already ordered
        integral1 = integral(f, x_fh, x_gh, 0.0001) - integral(h, x_fh, x_gh, 0.0001); // left part
        if (integral1 < 0) {
            integral1 *= -1;
        }
 
        integral2 = integral(f, x_gh, x_fg, 0.0001) - integral(g, x_gh, x_fg, 0.0001); // right part
        if (integral2 < 0) {
            integral2 *= -1;
        } 
 
        double area; // total area
        area = integral1 + integral2;
 
        printf("%lf\n", area);
 
    } else if (c == 'h') { // list of options
        printf("available options:\n");
        printf("--help (-h)\n");
        printf("--root (-r)\n");
        printf("--iterations (-i)\n");
        printf("--test-root F1:F2:A:B:E:R (-R F1:F2:A:B:E:R)\n");
        printf("--test-integral F:A:B:E:R (-I F:A:B:E:R)\n");
    } else if (c == 'r') {
        double x_fg, x_fh, x_gh;
        // calculating the intersections
        x_fg = root(f, g, 1, 2, 0.00004, f_, g_);
        x_fh = root(f, h, -3, -2, 0.00004, f_, h_);
        x_gh = root(g, h, 0.6, 1, 0.00004, g_, h_);
 
        printf("f intersects with g at x = %lf\n", x_fg);
        printf("f intersects with h at x = %lf\n", x_fh);
        printf("g intersects with h at x = %lf\n", x_gh);
    } else if (c == 'i') {
        // calculating number of iterations inside root function
        root(f, g, 1, 2, 0.00004, f_, g_);
        printf("calculating the point of intersection of f and g took %d iterations\n", iterations);
        iterations = 0;
        root(f, h, -3, -2, 0.00004, f_, h_);
        printf("calculating the point of intersection of f and h took %d iterations\n", iterations);
        iterations = 0;
        root(g, h, 0.6, 1, 0.00004, g_, h_);
        printf("calculating the point of intersection of g and h took %d iterations\n", iterations);
    } else if (c == 'R') {
        // testing root function
        int F1, F2; // test function numbers
        double A, B, E, R; // calculating on [A, B] with precision = E, correct answer = R
        sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &F1, &F2, &A, &B, &E, &R);
        double x;
        x = root(test_functions[F1 - 1], test_functions[F2 - 1], A, B, E, test_derivatives[F1 - 1], test_derivatives[F2 - 1]); // calculating the root
        printf("%lf ", x);
        double abs_e = x - R; // absolute error
        if (abs_e < 0) {
            abs_e *= -1;
        }
        printf("%lf ", abs_e);
        double rel_e; // relative error
        if (0.0000000001 < R || R < -0.0000000001) {
            rel_e = abs_e / R; 
        }  else {
            rel_e = 0;
        }
        if (rel_e < 0) {
            rel_e *= -1;
        }
        printf("%lf\n", rel_e);
 
    } else if (c == 'I') {
        // testing integral function
        int F; // test function number
        double A, B, E, R; // calculating on [A, B] with precision = E, correct answer = R
        sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &F, &A, &B, &E, &R);
        double x;
        x = integral(test_functions[F - 1], A, B, E); // calculating the integral
        printf("%lf ", x);
        double abs_e = x - R; // absolute error
        if (abs_e < 0) {
            abs_e *= -1;
        }
        printf("%lf ", abs_e);
        double rel_e; // relative error
        if (0.0000000001 < R || R < -0.0000000001) {
            rel_e = abs_e / R; 
        }  else {
            rel_e = 0;
        }
        if (rel_e < 0) {
            rel_e *= -1;
        }
        printf("%lf\n", rel_e);
    } else {
        printf("%c\n", c);
    }
 
    return 0;
}