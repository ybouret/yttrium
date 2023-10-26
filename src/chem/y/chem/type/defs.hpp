
//! \file


#ifndef Y_Chemical_Types_Included
#define Y_Chemical_Types_Included 1

#include "y/mkl/antelope/mul.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef XReal<double>             xreal; //!< internal computations
        typedef MKL::Antelope::Mul<xreal> XMul;  //!< precise mul
        typedef MKL::Antelope::Add<xreal> XAdd;  //!< precise add

        //! conversion helper to display
        struct Conv
        {
            //! xreal to double
            static inline double X2R(const xreal &x) { return double(x); }

            //! identify
            template <typename T>
            static inline const T &Id(const T &x) { return x; }
        };


    }
}



#endif
