

#include "y/mkl/root/zfind.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {

        
#define real_t float
#include "zfind.hxx"

#undef  real_t
#define real_t double
#include "zfind.hxx"

#undef  real_t
#define real_t long double
#include "zfind.hxx"

#undef  real_t
#define real_t XReal<float>
#include "zfind.hxx"

#undef  real_t
#define real_t XReal<double>
#include "zfind.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "zfind.hxx"
    }

}


