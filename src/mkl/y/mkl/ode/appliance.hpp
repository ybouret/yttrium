//! \file

#ifndef Y_ODE_Appliance_Included
#define Y_ODE_Appliance_Included 1

#include "y/mkl/ode/integrator.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace ODE
        {

            template <typename T>
            class Appliance :
            public Actuator<T>::Pointer,
            public Integrator<T>
            {
            public:
                typedef typename Actuator<T>::Pointer Scheme;

                //!
                /**
                 \param usrEps initial tolerance
                 \param usrDim initial dimension
                 */
                explicit Appliance(const Scheme &scheme,
                                   const T       usrEps,
                                   const size_t  usrDim);

                explicit Appliance(Actuator<T> *actuator,
                                   const T      usrEps,
                                   const size_t usrDim);

                virtual ~Appliance() noexcept;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Appliance);
            };

            
        }
    }
}

#endif

