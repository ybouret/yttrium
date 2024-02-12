//! \file

#ifndef Y_MKL_Numeric_Included
#define Y_MKL_Numeric_Included 1

#include "y/mkl/xreal.hpp"
#include <cmath>

namespace Yttrium
{
    namespace MKL
    {


        
        //______________________________________________________________________
        //
        //
        //! numeric constants
        //
        //______________________________________________________________________
        template <typename T>
        struct Numeric
        {
            static const unsigned RADIX;        //!< RADIX
            static const T        PI;           //!< PI
            static const T        EPSILON;      //!< EPSILON
            static const T        MAX;          //!< MAX
            static const T        MIN;          //!< MIN
            static const unsigned MANT_DIG;     //!< MANT_DIG
            static const unsigned DIG;          //!< DIGits
            static const int      MIN_EXP;      //!< MIN_EXP
            static const int      MAX_EXP;      //!< MAX_EXP
            static const int      MAX_10_EXP;   //!< MAX_10_EXP
            static const int      MIN_10_EXP;   //!< MIN_10_EXP

            static const T        FTOL;         //!< 10^(-DIG)
            static const T        SQRT_EPSILON; //!< sqrt(EPSILON)
            static const T        GOLDEN;       //!< (1+sqrt(5))/2
            static const T        INV_GOLDEN;   //!< 1/GOLDEN = GOLDEN-1 = (sqrt(5)-1)/2
        };

#if !defined(_MSC_VER)
        //! helper to declare existence of values
#define Y_MKL_NUMERIC(VAR) \
template <> const float              Numeric<float>                :: VAR; \
template <> const double             Numeric<double>               :: VAR; \
template <> const long double        Numeric<long double>          :: VAR; \
template <> const XReal<float>       Numeric< XReal<float> >       :: VAR; \
template <> const XReal<double>      Numeric< XReal<double> >      :: VAR; \
template <> const XReal<long double> Numeric< XReal<long double> > :: VAR

        Y_MKL_NUMERIC(PI);           //!< declare PI
        Y_MKL_NUMERIC(EPSILON);      //!< declare EPSILON
        Y_MKL_NUMERIC(MIN);          //!< declare MIN
        Y_MKL_NUMERIC(MAX);          //!< declare MIN
        Y_MKL_NUMERIC(FTOL);         //!< declare FTOL
        Y_MKL_NUMERIC(SQRT_EPSILON); //!< declare SQRT_EPSILON
        Y_MKL_NUMERIC(GOLDEN);       //!< declare GOLDEN
        Y_MKL_NUMERIC(INV_GOLDEN);   //!< declare INV_GOLDEN

#endif

    }
}

#endif

