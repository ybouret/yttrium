

#include "y/mkl/root/zrid.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/sort/nw.hpp"

namespace Yttrium
{
    namespace MKL
    {

        static const char CallSign[] = "MKL::ZRid";

        
#define real_t float
#include "zrid.hxx"

#undef  real_t
#define real_t double
#include "zrid.hxx"

#undef  real_t
#define real_t long double
#include "zrid.hxx"

#undef  real_t
#define real_t XReal<float>
#include "zrid.hxx"

#undef  real_t
#define real_t XReal<double>
#include "zrid.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "zrid.hxx"
    }




}


