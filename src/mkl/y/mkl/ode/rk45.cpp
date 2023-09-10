
#include "y/mkl/ode/rk45.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {


            template <typename T>
            class RK45<T>:: Code : public Object
            {
            public:
                typedef CxxArray<T,Memory::Dyadic>  ArrayType;
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;
                
                inline explicit Code(const size_t n) :
                dims(n),
                yerr(n),
                ytemp(n)
                {
                    assert(dims>0);
                }

                inline virtual ~Code() noexcept
                {
                }

                inline void move(RK45_Step<T> & step,
                                 Writable<T>  & y,
                                 Writable<T>  & dydx,
                                 T              x,
                                 T              htry,
                                 const T        eps,
                                 Writable<T> &  yscal,
                                 T  &           hdid,
                                 T  &           hnext,
                                 Equation &     drvs,
                                 Callback *     cntl)
                {

                    T h = htry;
                    while(true)
                    {
                        
                    }
                }


                const size_t dims;
                ArrayType yerr, ytemp;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

#define real_t float
#include "rk45.hxx"

#undef  real_t
#define real_t double
#include "rk45.hxx"

#undef  real_t
#define real_t long double
#include "rk45.hxx"

#undef  real_t
#define real_t XReal<float>
#include "rk45.hxx"

#undef  real_t
#define real_t XReal<double>
#include "rk45.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "rk45.hxx"
        }

    }

}


