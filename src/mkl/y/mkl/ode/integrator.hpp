
//! file

#ifndef Y_ODE_Integrator_Included
#define Y_ODE_Integrator_Included 1

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
            //! Drive an Actuator to integrate a differential equation
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Integrator
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
                explicit Integrator(const T userEps) noexcept;        //!< setup empty
                explicit Integrator(const T userEps,const size_t n);  //!< setup with capacity
                virtual ~Integrator() noexcept;                       //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! integration over an interval
                /**
                 \param ystart   initial phase space, updated
                 \param x1       initial position
                 \param x2       final  position
                 \param hini     guess step size, updated
                 \param drvs     differential equation
                 \param cntl     differential control
                 \param actuator actuator
                 */
                //______________________________________________________________

                void run(Writable<T> & ystart,
                         const T       x1,
                         const T       x2,
                         T           & hini,
                         Equation    & drvs,
                         Callback    * cntl,
                         Actuator<T> & actuator);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                T eps; //!< tolerance within algorithm
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Integrator);
                class Code;
                Code *code;
            };

        }

    }

}

#endif
