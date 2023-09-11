
#ifndef Y_MKL_ODE_CashKarp_Included
#define Y_MKL_ODE_CashKarp_Included 1


#include "y/mkl/ode/types.hpp"
#include "y/counted.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace ODE
        {

            //__________________________________________________________________
            //
            //
            //! Interface for RK45 Steps
            //
            //__________________________________________________________________
            template <typename T>
            class RK45_Step : public Object, public Counted
            {
            public:
                //______________________________________________________________
                //
                //
                //  Definitions
                //
                //______________________________________________________________
                typedef typename Named<T>::Equation Equation; //!< alias
                typedef typename Named<T>::Callback Callback; //!< alias
                typedef ArcPtr<RK45_Step>           Handle;   //!< alias

                //______________________________________________________________
                //
                //! interface to guess next positiuon
                /**
                 \param y    initial phase space
                 \param dydx initial derivaties
                 \param x    initial parameter
                 \param h    guess step
                 \param yout guess output phase space
                 \param yerr guess output error
                 \param drvs differential equation
                 \param cntl differential control
                */
                //______________________________________________________________
                virtual void operator()(const Readable<T> & y,
                                        const Readable<T> & dydx,
                                        const T             x,
                                        const T             h,
                                        Writable<T> &       yout,
                                        Writable<T> &       yerr,
                                        Equation     &      drvs,
                                        Callback     *      cntl) = 0;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~RK45_Step() noexcept {} //!< cleanup
            protected:
                explicit RK45_Step() noexcept {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK45_Step);
            };

            //__________________________________________________________________
            //
            //
            //! Cash Karp 4/5 step
            //
            //__________________________________________________________________
            template <typename T>
            class CashKarp : public RK45_Step<T>
            {
            public:
                //______________________________________________________________
                //
                //
                //  Definitions
                //
                //______________________________________________________________
                typedef typename Named<T>::Equation Equation; //!< alias
                typedef typename Named<T>::Callback Callback; //!< alias

                //______________________________________________________________
                //
                //
                //  C++
                //
                //______________________________________________________________
                explicit CashKarp() noexcept;      //!< setup empty
                explicit CashKarp(const size_t n); //!< setup with capacity
                virtual ~CashKarp() noexcept;      //!< cleanup

                //______________________________________________________________
                //
                //
                //!  Interface
                //
                //______________________________________________________________
                void operator()(const Readable<T> & y,
                                const Readable<T> & dydx,
                                const T             x,
                                const T             h,
                                Writable<T> &       yout,
                                Writable<T> &       yerr,
                                Equation     &      drvs,
                                Callback     *      cntl);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CashKarp);
                class Code;
                Code *code;
            };
        }
    }

}

#endif

