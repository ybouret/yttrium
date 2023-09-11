
//! file

#ifndef Y_ODE_Actuator_Included
#define Y_ODE_Actuator_Included 1

#include "y/mkl/ode/types.hpp"

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
            //! Equation actuator to move phase space
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Actuator
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef typename Named<T>::Equation   Equation; //!< alias
                typedef typename Named<T>::Callback   Callback; //!< alias


                //______________________________________________________________
                //
                //! interface to move phase space
                /**
                 \param y     initial then final phase space
                 \param dydx  initial derivatives
                 \param x     initial then final parameters
                 \param htry  trial step
                 \param eps   fractional tolerance on phase-space
                 \param yscal scaling of phase space variations
                 \param hdid  x += hdid
                 \param hnxt  estimated next possible step size
                 \param drvs differential equation
                 \param cntl differential control
                 */
                //_____________________________________________________________

                virtual void move(Writable<T>  &       y,
                                  const Readable<T>  & dydx,
                                  T                  & x,
                                  const T              htry,
                                  const T              eps,
                                  const Readable<T> &  yscal,
                                  T  &                 hdid,
                                  T  &                 hnxt,
                                  Equation &           drvs,
                                  Callback *           cntl) = 0;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~Actuator() noexcept {} //!< cleanup

            protected:
                inline explicit Actuator( ) noexcept {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Actuator);
            };
        }

    }

}

#endif
