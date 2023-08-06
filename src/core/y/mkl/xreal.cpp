
#include "y/mkl/xreal.hpp"
#include <cmath>

#define real_t float
#include "xreal.hxx"

#undef real_t
#define real_t double
#include "xreal.hxx"

#undef real_t
#define real_t long double
#include "xreal.hxx"

