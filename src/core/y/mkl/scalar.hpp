
//! \file

#ifndef Y_MKL_Scalar_Included
#define Y_MKL_Scalar_Included 1

#include "y/mkl/complex.hpp"

namespace Yttrium
{

    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //! Default Scalar Type
        //
        //______________________________________________________________________
        template <typename T>
        struct ScalarFor
        {
            typedef T Type; //!< self type
        };

        //______________________________________________________________________
        //
        //
        //! Scalar type for Complexes
        //
        //______________________________________________________________________
        template <typename T>
        struct ScalarFor< Complex<T> >
        {
            typedef T Type; //!< base type
        };


    }

}

#endif

