

#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/api.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace MKL
    {
        static const char CallSign[] = "[Bracketing]";

        bool Bracketing::Verbose = false;

#define Y_BRACKET_PRINT(MSG) do { if(Bracketing::Verbose) { std::cerr << CallSign << ' ' << MSG << std::endl; } } while(false)

#define real_t float
#include "bracket.hxx"

#undef  real_t
#define real_t double
#include "bracket.hxx"

#undef  real_t
#define real_t long double
#include "bracket.hxx"

#undef  real_t
#define real_t XReal<float>
#include "bracket.hxx"

#undef  real_t
#define real_t XReal<double>
#include "bracket.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "bracket.hxx"
    }

}


