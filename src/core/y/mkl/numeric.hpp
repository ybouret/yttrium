//! \file

#ifndef Y_MKL_Numeric_Included
#define Y_MKL_Numeric_Included 1

#include "y/config/starting.hpp"
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
            static const T PI;      //!< PI
            static const T EPSILON; //!< epsilon
        };

#if !defined(_MSC_VER)
        //! helper to declare existence of values
#define Y_MKL_NUMERIC(VAR) \
template <> const float       Numeric<float>      :: VAR;\
template <> const double      Numeric<double>     :: VAR;\
template <> const long double Numeric<long double>:: VAR

        Y_MKL_NUMERIC(PI);         //!< declare PI
        Y_MKL_NUMERIC(EPSILON);    //!< declare EPSILON
#endif

    }
}

#endif

