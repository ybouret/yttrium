
//! \file

#ifndef Y_MKL_ODE_MidPoint_Included
#define Y_MKL_ODE_MidPoint_Included 1

#include "y/mkl/ode/types.hpp"
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
            //! Midpoint updating
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MidPoint
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef typename Named<T>::Equation Equation; //!< alias
                typedef typename Named<T>::Callback Callback; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit MidPoint() noexcept;         //!< setup
                explicit MidPoint(const size_t nvar); //!< setup with memory
                virtual ~MidPoint() noexcept;         //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! estimate yout with nstep over [x:x+h]
                //______________________________________________________________
                void compute(const Readable<T> &y,
                             const Readable<T> &dydx,
                             const T            x,
                             const T            h,
                             const size_t       nstep,
                             Writable<T>       &yout,
                             Equation          &drvs,
                             Callback          *cntl);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MidPoint);
                class Code;
                Code *code;
            };
        }

    }

}

#endif
