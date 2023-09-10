
#ifndef Y_MKL_ODE_CashKarp_Included
#define Y_MKL_ODE_CashKarp_Included 1


#include "y/mkl/ode/types.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace ODE
        {
            template <typename T>
            class CashKarp
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

