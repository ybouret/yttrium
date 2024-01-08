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
                template <
                template <typename> class STEP,
                typename T
                >
                class Scheme :
                private Step<T>::Pointer,
                public  Controller<T>
                {
                public:
                    typedef  typename Step<T>::Pointer StepType;

                    explicit Scheme(const size_t n) : 
                    StepType( new STEP<T>(n) ),
                    Controller<T>(**this, n)
                    {}

                    virtual ~Scheme() noexcept {}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Scheme);
                };
            }
        }

    }

}

#endif
