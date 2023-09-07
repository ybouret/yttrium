
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

        template <> const unsigned Numeric<float>::      RADIX = FLT_RADIX;
        template <> const unsigned Numeric<double>::     RADIX = FLT_RADIX;
        template <> const unsigned Numeric<long double>::RADIX = FLT_RADIX;


#define Y_MKL_DECL_TYPE(TYPE,VAR) \
        template <> const TYPE Numeric<float> ::      VAR = FLT_##VAR; \
        template <> const TYPE Numeric<double>::      VAR = DBL_##VAR; \
        template <> const TYPE Numeric<long double>:: VAR = LDBL_##VAR

        Y_MKL_DECL_TYPE(unsigned,MANT_DIG);
        Y_MKL_DECL_TYPE(unsigned,DIG);

        Y_MKL_DECL_TYPE(int,MIN_EXP);
        Y_MKL_DECL_TYPE(int,MAX_EXP);


#define Y_MKL_DECL_REAL(VAR) \
template <> const float       Numeric<float> ::      VAR = FLT_##VAR; \
template <> const double      Numeric<double>::      VAR = DBL_##VAR; \
template <> const long double Numeric<long double>:: VAR = LDBL_##VAR

        Y_MKL_DECL_REAL(EPSILON);
        Y_MKL_DECL_REAL(MIN);
        Y_MKL_DECL_REAL(MAX);


        template <> const float       Numeric<float>       :: FTOL = powf( 10.0f,-int(DIG));
        template <> const double      Numeric<double>      :: FTOL = pow(  10.0, -int(DIG));
        template <> const long double Numeric<long double> :: FTOL = powl( 10.0l,-int(DIG));

    }

}

