//! \file

#ifndef Y_MKL_ODE_RK45_Step_Included
#define Y_MKL_ODE_RK45_Step_Included 1


#include "y/mkl/ode/types.hpp"
#include "y/counted.hpp"
#include "y/ptr/arc.hpp"

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
                //! Interface for RK45 Steps
                //
                //__________________________________________________________________
                template <typename T>
                class  Step : public Object, public Counted
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
                    typedef ArcPtr<Step>                Pointer;  //!< alias

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
                    virtual ~Step() noexcept; //!< cleanup
                protected:
                    explicit Step() noexcept;   //!< setup

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Step);
                };
            }
        }

    }

}

#endif

