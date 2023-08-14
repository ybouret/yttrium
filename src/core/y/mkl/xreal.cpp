
#include "y/mkl/xreal.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/numeric.hpp"
#include "y/type/signs.hpp"

#include <cerrno>
#include <cmath>

#define real_t float
#include "xreal.hxx"

#undef real_t
#define real_t double
#include "xreal.hxx"

#undef real_t
#define real_t long double
#include "xreal.hxx"

