
#include "y/mkl/numeric.hpp"
#include <cmath>
#include <cfloat>
#include <climits>

namespace Yttrium
{
    namespace MKL
    {
        template <> const float       Numeric<float> ::      PI = M_PI;
        template <> const double      Numeric<double>::      PI = M_PI;
        template <> const long double Numeric<long double>:: PI = M_PI;

    }

}

