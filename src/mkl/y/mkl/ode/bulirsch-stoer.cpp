#include "y/mkl/ode/bulirsch-stoer.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace ODE
        {
            template <typename T>
            class BulirschStoer<T>:: Code : public Object
            {
            public:

                inline explicit Code()
                {
                }

                inline virtual ~Code() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };


#define real_t float
#include "bulirsch-stoer.hxx"

#undef  real_t
#define real_t double
#include "bulirsch-stoer.hxx"

#undef  real_t
#define real_t long double
#include "bulirsch-stoer.hxx"

        }

    }
}

