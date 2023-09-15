

#include "y/mkl/ode/integrator.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/nullify.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {

            template <typename T>
            class Integrator<T>:: Code : public Object
            {
            public:
                typedef CxxArray<T,Memory::Dyadic>  ArrayType;
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;

                inline explicit Code(const size_t n) :
                Object(),
                dims(n),
                y(n),
                dydx(n),
                yscal(n)
                {
                    assert(dims>0);
                }

                inline virtual ~Code() noexcept {}

                inline void update(Writable<T> &ystart,
                                   const T      x1,
                                   const T      x2,
                                   T           &hini,
                                   Equation    &drvs,
                                   Callback    *cntl,
                                   Actuator<T> &actuator,
                                   const T      eps)
                {
                    static const T tiny(1e-30);
                    static const T zero(0);
                    assert(hini>zero);
                    const size_t   n          = ystart.size(); assert(dims>=n);
                    T              h          = zero;
                    bool           increasing = true;
                    switch(Sign::Of(x2,x1))
                    {
                        case __Zero__: return;
                        case Positive: increasing=true;  h =  Fabs<T>::Of(hini); break;
                        case Negative: increasing=false; h = -Fabs<T>::Of(hini); break;
                    }

                    T x = x1;
                    for(size_t i=n;i>0;--i)
                        y[i] = ystart[i];

                    while(true)
                    {
                        // evaluate derivatives
                        drvs(dydx,x,y);

                        // compute step/xnext
                        bool wouldFinish = false;
                        T xnext = x+h;
                        if(increasing)
                        {
                            if(xnext>=x2) { xnext=x2; wouldFinish=true; }
                        }
                        else
                        {
                            if(xnext<=x1) { xnext=x1; wouldFinish=true; }
                        }
                        h = xnext - x;

                        // evaluate scaling
                        for(size_t i=n;i>0;--i)
                            yscal[i] = Fabs<T>::Of(y[i]) + Fabs<T>::Of(h*dydx[i]) + tiny;

                        // take a controlled step: y and x are updated
                        T hdid = 0, hnxt=0;
                        actuator.move(y, dydx, x, h, eps, yscal, hdid, hnxt, drvs, cntl);

                        // are we done ?
                        if( wouldFinish && Fabs<T>::Of(hdid-h) <= zero )
                        {
                            for(size_t i=n;i>0;--i)
                                ystart[i] = y[i];
                            hini = Fabs<T>::Of(hnxt);
                            return;
                        }

                        // x is updated, update h for next cycle
                        h = hnxt;
                    }





                }

                

                const size_t dims;
                ArrayType    y;
                ArrayType    dydx;
                ArrayType    yscal;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };



#define real_t float
#include "integrator.hxx"

#undef  real_t
#define real_t double
#include "integrator.hxx"

#undef  real_t
#define real_t long double
#include "integrator.hxx"

#undef  real_t
#define real_t XReal<float>
#include "integrator.hxx"

#undef  real_t
#define real_t XReal<double>
#include "integrator.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "integrator.hxx"


        }

    }

}


