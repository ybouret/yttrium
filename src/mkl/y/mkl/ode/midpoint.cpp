
#include "y/mkl/ode/midpoint.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/antelope/sum3.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace ODE
        {

            template <typename T>
            class MidPoint<T> :: Code : public Object
            {
            public:
                typedef CxxArray<T,Memory::Dyadic>  ArrayType;
                typedef Antelope::Sum3Proxy<T,true> Sum3;

                explicit Code(const size_t n) :
                dims(n),
                yn(n),
                ym(n)
                {
                    assert(dims>0);
                }
                virtual ~Code() noexcept {}

                inline void compute(const Readable<T> &y,
                                    const Readable<T> &dydx,
                                    const T            xs,
                                    const T            htot,
                                    const size_t       nstep,
                                    Writable<T>       &yout,
                                    Equation          &drvs,
                                    Callback          *cntl)
                {
                    static const T half(0.5);
                    const size_t   nvar = y.size(); assert(nvar<=dims);
                    const T        h    = htot/T(nstep);

                    for(size_t i=nvar;i>0;--i)
                    {
                        ym[i]=y[i];
                        yn[i]=y[i]+h*dydx[i];
                    }

                    T x=xs+h;
                    if(cntl) (*cntl)(x,yn);
                    drvs(yout,x,yn);

                    const T h2=h+h;
                    for(size_t n=2;n<=nstep;n++)
                    {
                        for(size_t i=nvar;i>0;--i)
                        {
                            const T swap=ym[i]+h2*yout[i];
                            ym[i]=yn[i];
                            yn[i]=swap;
                        }
                        x += h;
                        if(cntl) (*cntl)(x,yn);
                        drvs(yout,x,yn);
                    }

                    for(size_t i=nvar;i>0;--i)
                    {
                        //yout[i]=half*(ym[i]+yn[i]+h*yout[i]);
                        const T incr = h * yout[i];
                        yout[i] = half * Sum3::Of(ym[i],yn[i],incr);
                    }
                    if(cntl) (*cntl)(xs+htot,yout);
                }


                const size_t dims;
                ArrayType    yn;
                ArrayType    ym;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };


#define real_t float
#include "midpoint.hxx"

#undef  real_t
#define real_t double
#include "midpoint.hxx"

#undef  real_t
#define real_t long double
#include "midpoint.hxx"

#undef  real_t
#define real_t XReal<float>
#include "midpoint.hxx"

#undef  real_t
#define real_t XReal<double>
#include "midpoint.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "midpoint.hxx"

        }

    }
}


