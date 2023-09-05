//! \file

#ifndef Y_MKL_ODE_RK4_Included
#define Y_MKL_ODE_RK4_Included 1

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
            //! Runge-Kutta-4 algorithm
            //
            //
            //__________________________________________________________________
            template <typename T>
            class RK4
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
                explicit RK4() noexcept;         //!< setup
                explicit RK4(const size_t nvar); //!< setup with memory
                virtual ~RK4() noexcept;         //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! step according to RK4
                void operator()(Writable<T> &Y,
                                Equation    &eqs,
                                const T      t0,
                                const T      t1,
                                Callback    *cb = 0);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK4);
                class Code;
                Code *code;
            };
        }

    }

}

#endif

