
//! \file

#ifndef Y_MKL_ODE_Types_Included
#define Y_MKL_ODE_Types_Included 1

#include "y/functor.hpp"
#include "y/container/writable.hpp"
#include <iostream>

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {
            //__________________________________________________________________
            //
            //
            //
            //! Interfaces for ODEs
            //
            //
            //__________________________________________________________________
            template <typename T>
            struct Field
            {
                typedef Functor<void,TL3(Writable<T> &,T,const Readable<T> &)> Equation; //!< equation interface
                typedef Functor<void,TL2(T,Writable<T>)>                       Callback; //!< callback interface
            };

        }
    }
}

#endif
