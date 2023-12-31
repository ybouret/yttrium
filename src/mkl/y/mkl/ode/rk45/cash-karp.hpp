//! \file

#ifndef Y_MKL_ODE_CashKarp_Included
#define Y_MKL_ODE_CashKarp_Included 1


#include "y/mkl/ode/rk45/step.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace ODE
        {

            namespace RK45
            {
                //__________________________________________________________________
                //
                //
                //! Cash Karp 4/5 step
                //
                //__________________________________________________________________
                template <typename T>
                class CashKarp : public Step<T>
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

}

#endif

