//! file

#ifndef Y_ODE_RK45_Included
#define Y_ODE_RK45_Included 1

#include "y/mkl/ode/rk45/step.hpp"
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
            //! adaptive, controled step
            //
            //
            //__________________________________________________________________
            template <typename T>
            class RK45 : public Actuator<T>
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
                typedef typename RK45_Step<T>::Handle StepType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit RK45(const StepType &s) noexcept;        //!< setup empty
                explicit RK45(const StepType &s, const size_t n); //!< setup for dim=n
                virtual ~RK45() noexcept;                         //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! move y to a new controlled value
                virtual void move(Writable<T>  &       y,
                                  const Readable<T>  & dydx,
                                  T                  & x,
                                  const T              htry,
                                  const T              eps,
                                  const Readable<T> &  yscal,
                                  T  &                 hdid,
                                  T  &                 hnxt,
                                  Equation &           drvs,
                                  Callback *           cntl);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK45);
                class    Code;
                StepType step;
                Code *   code;
            };
        }
    }
}

#endif

