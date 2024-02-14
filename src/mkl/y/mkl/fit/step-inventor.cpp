
#include "y/mkl/fit/step-inventor.hpp"
#include "y/calculus/ipower.hpp"
#include "y/mkl/numeric.hpp"

#define real_t float
#include "step-inventor.hxx"
#undef real_t

#define real_t double
#include "step-inventor.hxx"
#undef real_t

#define real_t long double
#include "step-inventor.hxx"
#undef real_t

#define real_t XReal<float>
#include "step-inventor.hxx"
#undef real_t

#define real_t XReal<double>
#include "step-inventor.hxx"
#undef real_t

#define real_t XReal<long double>
#include "step-inventor.hxx"
#undef real_t
