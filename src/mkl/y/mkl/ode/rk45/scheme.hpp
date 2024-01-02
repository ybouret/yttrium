
//! \file

#ifndef Y_MKL_ODE_RK45_Scheme_Included
#define Y_MKL_ODE_RK45_Scheme_Included 1


#include "y/mkl/ode/rk45/controller.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace ODE
        {

            namespace RK45
            {

#if 0
                template <
                template <typename T> class STEP,
                typename                    T>
                class Scheme :
                public Step<T>::Handle,
                public Controller<T>
                {
                public:
                    typedef typename Step<T>::Handle StepType;

                    inline explicit Scheme(const size_t dims) :
                    StepType( new STEP<T>() ),
                    Controller<T>( static_cast<const StepType &>(*this), dims )
                    {}
                    
                    inline virtual ~Scheme() noexcept {}


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Scheme);
                };
#endif
            }
        }

    }

}

#endif
