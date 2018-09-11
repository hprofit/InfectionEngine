
#ifndef PRECISION_H
#define PRECISION_H

#include <float.h>

namespace physics {

    #define DOUBLE_PRECISION
    typedef float real;
    #define REAL_MAX FLT_MAX
    #define real_sqrt sqrt
    #define real_abs fabs
    #define real_sin sin
    #define real_cos cos
    #define real_exp exp
    #define real_pow pow
    #define real_fmod fmod
    #define real_epsilon DBL_EPSILON
    #define R_PI 3.14159265358979f
}

#endif 
