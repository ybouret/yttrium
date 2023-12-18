
#include "y/mkl/eigen/balance.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/utils.hpp"
#include "y/mkl/numeric.hpp"
//#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Eigen
        {
#define real_t float
#include "balance.hxx"

#undef  real_t
#define real_t double
#include "balance.hxx"

#undef  real_t
#define real_t long double
#include "balance.hxx"

#undef  real_t
#define real_t XReal<float>
#include "balance.hxx"

#undef  real_t
#define real_t XReal<double>
#include "balance.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "balance.hxx"
        }

    }

}

