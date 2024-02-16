
#include "y/mkl/fit/least-squares/com.hpp"

#define real_t float
#include "com.hxx"
#undef real_t

#define real_t double
#include "com.hxx"
#undef real_t

#define real_t long double
#include "com.hxx"
#undef real_t

#define real_t XReal<float>
#include "com.hxx"
#undef real_t

#define real_t XReal<double>
#include "com.hxx"
#undef real_t

#define real_t XReal<long double>
#include "com.hxx"
#undef real_t
