

#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/opt/parabolic.hpp"
#include "y/mkl/api.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace MKL
    {
        //static const char CallSign[] = "Parabolic::Step";

#define real_t float
#include "minimize.hxx"

#undef  real_t
#define real_t double
#include "minimize.hxx"

#undef  real_t
#define real_t long double
#include "minimize.hxx"

#undef  real_t
#define real_t XReal<float>
#include "minimize.hxx"

#undef  real_t
#define real_t XReal<double>
#include "minimize.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "minimize.hxx"
    }

}


