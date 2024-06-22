//! \file

#ifndef Y_MKL_Antelope_Wary_Included
#define Y_MKL_Antelope_Wary_Included 1

#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {

            //__________________________________________________________________
            //
            //
            //
            //! Default Wary Flag is true
            //
            //
            //__________________________________________________________________
            template <typename T> struct Wary { enum { Flag = true }; };

            //__________________________________________________________________
            //
            //
            //
            //! Apex::Natural : no special treatment
            //
            //
            //__________________________________________________________________
            template <> struct Wary<apn> { enum { Flag = false }; };

            //__________________________________________________________________
            //
            //
            //
            //! Apex::Integer : no special treatment
            //
            //
            //__________________________________________________________________
            template <> struct Wary<apz> { enum { Flag = false }; };

            //__________________________________________________________________
            //
            //
            //
            //! Apex::Rational : no special treatment
            //
            //
            //__________________________________________________________________
            template <> struct Wary<apq> { enum { Flag = false }; };

            

        }
    }
}

#endif

