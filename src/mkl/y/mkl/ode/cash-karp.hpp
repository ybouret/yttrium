
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

            template <typename T>
            class RK45_Step : public Object, public Counted
            {
            public:
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;
                typedef ArcPtr<RK45_Step>           Handle;

                virtual ~RK45_Step() noexcept {}

                virtual void operator()(Writable<T> &       y,
                                        const Readable<T> & dydx,
                                        const T             x,
                                        const T             h,
                                        Writable<T> &       yout,
                                        Writable<T> &       yerr,
                                        Equation     &      drvs,
                                        Callback     *      cntl) = 0;

            protected:
                explicit RK45_Step() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK45_Step);
            };

            template <typename T>
            class CashKarp : public RK45_Step<T>
            {
            public:
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;
                
                explicit CashKarp() noexcept;
                explicit CashKarp(const size_t n);
                virtual ~CashKarp() noexcept;

                void operator()(Writable<T> &       y,
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

