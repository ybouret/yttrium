
#include "y/mkl/ode/cash-karp.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/tao.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {

            template <typename T>
            class CashKarp<T>:: Code : public Object
            {
            public:
                typedef CxxArray<T,Memory::Dyadic>  ArrayType;
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;


                inline explicit Code(const size_t n) :
                dims(n),
                ak2(n),
                ak3(n),
                ak4(n),
                ak5(n),
                ak6(n),
                ytemp(n),
                xadd(n)
                {
                    assert(dims>0);
                }

                inline virtual ~Code() noexcept {}

                inline void step(const Readable<T> & y,
                                 const Readable<T> & dydx,
                                 const T             x,
                                 const T             h,
                                 Writable<T> &       yout,
                                 Writable<T> &       yerr,
                                 Equation &          drvs,
                                 Callback *          cntl)
                {
                    static const T a2=0.2,a3=0.3,a4=0.6,a5=1.0,a6=0.875;
                    static const T b21=0.2, b31=3.0/40.0,b32=9.0/40.0,b41=0.3,b42 = -0.9,b43=1.2,
                    b51 = -11.0/54.0, b52=2.5,b53 = -70.0/27.0,b54=35.0/27.0, b61=1631.0/55296.0,b62=175.0/512.0,b63=575.0/13824.0, b64=44275.0/110592.0,b65=253.0/4096.0,c1=37.0/378.0, c3=250.0/621.0,c4=125.0/594.0,c6=512.0/1771.0,
                    dc5 = -277.00/14336.0;
                    static const T dc1=c1-T(2825.0/27648.0),dc3=c3-T(18575.0/48384.0),
                    dc4=c4-T(13525.0/55296.0),dc6=c6-T(0.25);

                    const size_t n = dydx.size(); assert(n<=dims);

                    {
                        const T fac = b21*h;
                        for (size_t i=n;i>0;--i)
                            ytemp[i]=y[i]+fac*dydx[i];
                    }

                    {
                        const T x2 = x+a2*h;
                        if(cntl) (*cntl)(x2,ytemp);
                        drvs(ak2,x2,ytemp);
                    }

                    for (size_t i=n;i>0;--i)
                        ytemp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);

                    {
                        const T x3=x+a3*h;
                        if(cntl) (*cntl)(x3,ytemp);
                        drvs(ak3,x3,ytemp);
                    }

                    for (size_t i=n;i>0;--i)
                    {
                        ytemp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
                    }

                    {
                        const T x4 = x+a4*h;
                        if(cntl) (*cntl)(x4,ytemp);
                        drvs(ak4,x4,ytemp);
                    }
                    for (size_t i=n;i>0;--i)
                        ytemp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);

                    {
                        const T x5 = x+a5*h;
                        if(cntl) (*cntl)(x5,ytemp);
                        drvs(ak5,x5,ytemp);
                    }
                    for (size_t i=n;i>0;--i)
                        ytemp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);

                    {
                        const T x6 = x+a6*h;
                        if(cntl) (*cntl)(x6,ytemp);
                        drvs(ak6,x6,ytemp);
                    }

                    for (size_t i=n;i>0;--i)
                        yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);

                    for (size_t i=n;i>0;--i)
                        yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);
                }


                const size_t dims;
                ArrayType    ak2, ak3, ak4, ak5, ak6, ytemp;
                Antelope::Add<T> xadd;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

#define real_t float
#include "cash-karp.hxx"

#undef  real_t
#define real_t double
#include "cash-karp.hxx"

#undef  real_t
#define real_t long double
#include "cash-karp.hxx"

#undef  real_t
#define real_t XReal<float>
#include "cash-karp.hxx"

#undef  real_t
#define real_t XReal<double>
#include "cash-karp.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "cash-karp.hxx"
        }

    }

}


