
#include "y/mkl/interpolation/linear.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{

    namespace MKL
    {
       


#define real_t float
#include "linear.hxx"

#undef  real_t
#define real_t double
#include "linear.hxx"

#undef  real_t
#define real_t long double
#include "linear.hxx"

#undef  real_t
#define real_t XReal<float>
#include "linear.hxx"

#undef  real_t
#define real_t XReal<double>
#include "linear.hxx"


#undef  real_t
#define real_t XReal<long double>
#include "linear.hxx"
    }

}


