#include "y/mkl/ode/rk4.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {

            //! RK4 Code for any T
            template <typename T>
            class RK4<T>:: Code : public Object
            {
            public:
                typedef CxxArray<T,Memory::Dyadic> Array;

                inline explicit Code(const size_t nvar) :
                dims(nvar),
                k1(nvar),
                k2(nvar),
                k3(nvar),
                k4(nvar),
                yt(nvar),
                xadd(nvar)
                {
                    assert(dims>0);
                }

                inline virtual ~Code() noexcept
                {
                }

                inline void step(Writable<T> &y,
                                 Equation    &eqs,
                                 const T      t0,
                                 const T      t1,
                                 Callback    *cb)
                {
                    assert(y.size()<=dims);
                    static const T half(0.5);
                    static const T six(6);

                    const T      h  = t1-t0;
                    const T      hh = half*h;
                    const T      th = t0 + hh;
                    const T      hs = h/six;
                    const size_t n  = y.size();

                    eqs(k1,t0,y);

                    for(size_t i=n;i>0;--i) yt[i] = y[i] + hh * k1[i];
                    if(cb) (*cb)(th,yt);
                    eqs(k2,th,yt);

                    for(size_t i=n;i>0;--i) yt[i] = y[i] + hh * k2[i];
                    if(cb) (*cb)(th,yt);
                    eqs(k3,th,yt);

                    for(size_t i=n;i>0;--i) yt[i] = y[i] + h * k3[i];
                    if(cb) (*cb)(t1,yt);
                    eqs(k4,t1,yt);

                    for(size_t i=n;i>0;--i)
                    {
                        xadd.free();
                        xadd << k1[i] << k2[i] << k2[i] << k3[i] << k3[i] << k4[i];
                        y[i] += hs * xadd.sum();
                    }
                    if(cb) (*cb)(t1,y);

                }
                

                const size_t     dims;
                Array            k1,k2,k3,k4,yt;
                Antelope::Add<T> xadd;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

#define real_t float
#include "rk4.hxx"

#undef  real_t
#define real_t double
#include "rk4.hxx"

#undef  real_t
#define real_t long double
#include "rk4.hxx"

#undef  real_t
#define real_t XReal<float>
#include "rk4.hxx"

#undef  real_t
#define real_t XReal<double>
#include "rk4.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "rk4.hxx"

        }
    }
}
