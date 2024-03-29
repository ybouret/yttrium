
#include "y/mkl/opt/parabolic.hpp"
#include "y/mkl/api.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/mkl/numeric.hpp"
#include "y/sort/indexing.hpp"


namespace Yttrium
{

    namespace MKL
    {
        static const char CallSign[] = "Parabolic::Step";

#define real_t float
#include "parabolic.hxx"

#undef  real_t
#define real_t double
#include "parabolic.hxx"

#undef  real_t
#define real_t long double
#include "parabolic.hxx"

#undef  real_t
#define real_t XReal<float>
#include "parabolic.hxx"

#undef  real_t
#define real_t XReal<double>
#include "parabolic.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "parabolic.hxx"
    }

}


