
#include "y/mkl/numeric.hpp"
#include <cmath>
#include <cfloat>
#include <climits>

namespace Yttrium
{
    namespace MKL
    {
#define Y_F(X) X##f
#define Y_D(X) X
#define Y_L(X) X##l
        
        template <> const float       Numeric<float> ::      PI = Y_F(3.14159265358979323846264338327950288419716939937510582);
        template <> const double      Numeric<double>::      PI = Y_D(3.14159265358979323846264338327950288419716939937510582);
        template <> const long double Numeric<long double>:: PI = Y_L(3.14159265358979323846264338327950288419716939937510582);

#define Y_MKL_DECL_REAL(VAR) \
template <> const float       Numeric<float> ::      VAR = FLT_##VAR; \
template <> const double      Numeric<double>::      VAR = DBL_##VAR; \
template <> const long double Numeric<long double>:: VAR = LDBL_##VAR

        Y_MKL_DECL_REAL(EPSILON);
    }

}

