//! \file

#ifndef Y_MKL_ODE_RK45_Scheme_Included
#define Y_MKL_ODE_RK45_Scheme_Included 1


#include "y/mkl/ode/rk45/cash-karp.hpp"
#include "y/mkl/ode/rk45/controller.hpp"
#include "y/mkl/ode/integrator.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace ODE
        {
            namespace RK45
            {

                //______________________________________________________________
                //
                //
                //
                //! RK45 all--in-one integrator
                //
                //
                //______________________________________________________________
                template <
                typename T,
                template <typename> class STEP = CashKarp
                >
                class Scheme :
                private Step<T>::Pointer,
                public  Controller<T>,
                public  ODE::Integrator<T>
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    typedef typename Step<T>::Pointer   StepType; //!< alias
                    typedef typename Named<T>::Equation Equation; //!< alias
                    typedef typename Named<T>::Callback Callback; //!< alias
                    using ODE::Integrator<T>::run;

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! setup resources
                    explicit Scheme(const size_t usrDim,
                                    const T      usrEps) :
                    StepType( new STEP<T>(usrDim) ),
                    Controller<T>(static_cast<const StepType&>(*this), usrDim),
                    ODE::Integrator<T>(usrEps,usrDim)
                    {}

                    //! cleanup
                    virtual ~Scheme() noexcept {}


                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________

                    //! call
                    inline void operator()(Writable<T> & ystart,
                                           const T       x1,
                                           const T       x2,
                                           T           & hini,
                                           Equation    & drvs,
                                           Callback    * cntl)
                    {
                        run(ystart,x1,x2,hini,drvs,cntl,*this);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Scheme);
                };
            }
        }

    }

}

#endif
