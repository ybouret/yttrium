

#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/api.hpp"
#include "y/mkl/numeric.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/mkl/antelope/sum3.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{

    namespace MKL
    {
        static const char CallSign[] = "Minimize::Step";

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


