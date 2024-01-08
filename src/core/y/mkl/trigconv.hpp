//! \file

#ifndef Y_MKL_TrigConv_Included
#define Y_MKL_TrigConv_Included 1

#include "y/mkl/numeric.hpp"

namespace Yttrium
{

    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Trigonometric Conversions
        //
        //
        //______________________________________________________________________
        struct TrigConv
        {
            //! Degrees to Radians
            template <typename T> static inline
            T DegToRad(const T deg)
            {
                static const T pi   = Numeric<T>::PI;
                static const T _180(180);
                return (pi*deg)/_180;
            }


            //! Radians to Degress
            template <typename T> static inline
            T RadToDeg(const T rad)
            {
                static const T pi   = Numeric<T>::PI;
                static const T _180(180);
                return (_180*rad)/pi;
            }

        };
    }
}

#endif

