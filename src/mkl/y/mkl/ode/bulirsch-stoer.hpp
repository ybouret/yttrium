
//! \file

#ifndef Y_MKL_ODE_Burlisch_Stoer_Included
#define Y_MKL_ODE_Burlisch_Stoer_Included 1

#include "y/mkl/ode/actuator.hpp"

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
            //! Bulirsch Stoer extrapolation methods
            //
            //
            //__________________________________________________________________
            template <typename T>
            class BulirschStoer : public Actuator<T>
            {
            public:
                typedef typename Named<T>::Equation Equation; //!< alias
                typedef typename Named<T>::Callback Callback; //!< alias

                explicit BulirschStoer() noexcept; //!< setup
                virtual ~BulirschStoer() noexcept; //!< cleanup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(BulirschStoer);
                class Code;
                Code *code;
            };

        }

    }

}

#endif
