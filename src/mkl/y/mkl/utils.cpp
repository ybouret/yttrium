#include "y/mkl/utils.hpp"

namespace Yttrium
{
    namespace MKL
    {
#define real_t float
#include "utils.hxx"

#undef  real_t
#define real_t double
#include "utils.hxx"

#undef  real_t
#define real_t long double
#include "utils.hxx"

#undef  real_t
#define real_t apq
#include "utils.hxx"


#undef  real_t
#define real_t XReal<float>
#include "utils.hxx"

#undef  real_t
#define real_t XReal<double>
#include "utils.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "utils.hxx"

#undef  real_t
#define real_t Complex<float>
#include "utils.hxx"

#undef  real_t
#define real_t Complex<double>
#include "utils.hxx"

#undef  real_t
#define real_t Complex<long double>
#include "utils.hxx"

#undef  real_t
#define real_t Complex< XReal<float> >
#include "utils.hxx"

#undef  real_t
#define real_t Complex< XReal<double> >
#include "utils.hxx"


#undef  real_t
#define real_t Complex< XReal<long double> >
#include "utils.hxx"
    }
}
