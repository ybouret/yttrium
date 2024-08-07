
#include "y/mkl/ode/rk45/controller.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/tao/seq/level1.hpp"
#include "y/type/nullify.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {
            
            namespace RK45
            {
                template <typename T>
                class Controller<T>:: Code : public Object
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
                    
                    inline void move(Step<T>      &       step,
                                     Writable<T>  &       y,
                                     const Readable<T>  & dydx,
                                     T                  & x,
                                     const T              htry,
                                     const T              eps,
                                     const Readable<T> &  yscal,
                                     T  &                 hdid,
                                     T  &                 hnxt,
                                     Equation &           drvs,
                                     Callback *           cntl)
                    {
                        static const T zero(0);
                        static const T one(1);
                        static const T oneTenth(0.1);
                        static const T five(5.0);
                        static const T SAFETY(0.9);
                        static const T PGROW(-0.2);
                        static const T PSHRNK(-0.25);
                        static const T ERRCON(1.89e-4);
                        
                        assert(y.size()<=dims);
                        const size_t n      = y.size();
                        T            h      = htry;
                        T            errmax = zero;
                        while(true)
                        {
                            step(y,dydx,x,h,ytemp,yerr,drvs,cntl);
                            
                            errmax=zero;
                            for(size_t i=1;i<=n;++i) errmax=Max(errmax,Fabs<T>::Of(yerr[i]/yscal[i]));
                            errmax /= eps;
                            //std::cerr << "errmax=" << errmax << std::endl;
                            if (errmax <= one) break;
                            
                            const T htemp = SAFETY*h*std::pow(errmax,PSHRNK); // reduce
                            h= (h >= zero ? Max(htemp,oneTenth*h) : Min(htemp,oneTenth*h));
                            volatile T xnew = x+h;
                            if( Fabs<T>::Of(xnew-x) <= zero ) throw Specific::Exception("RK45","stepsize underflow");
                        }

                        if (errmax > ERRCON) hnxt=SAFETY*h*std::pow(errmax,PGROW); // reduce
                        else                 hnxt=five*h;                          // increase
                        x += (hdid=h);
                        for(size_t i=n;i>0;--i) y[i]=ytemp[i];
                        //std::cerr << "hdid=" << hdid << " / hnxt=" << hnxt << std::endl;
                    }
                    
                    
                    const size_t dims;
                    ArrayType    yerr;
                    ArrayType    ytemp;
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Code);
                };
                
#define real_t float
#include "controller.hxx"
#undef  real_t

#define real_t double
#include "controller.hxx"
#undef  real_t

#define real_t long double
#include "controller.hxx"
#undef  real_t

#if 0
#undef  real_t
#define real_t XReal<float>
#include "rk45.hxx"
                
#undef  real_t
#define real_t XReal<double>
#include "rk45.hxx"
                
#undef  real_t
#define real_t XReal<long double>
#include "rk45.hxx"
#endif
            }
            
        }
    }

}


