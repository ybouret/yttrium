#include "y/mkl/derivatives/interface.hpp"
#include "y/mkl/api.hpp"
#include "y/mkl/triplet.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/mkl/numeric.hpp"



#include "y/system/exception.hpp"
#include "y/container/cxx-array.hpp"

#include "y/stream/libc/output.hpp"
#include "y/string.hpp"



namespace Yttrium
{
    namespace MKL
    {

        namespace Kernel
        {
            const char * const Derivatives:: CallSign = "Derivatives";

            Derivatives::  Derivatives() noexcept {}
            Derivatives:: ~Derivatives() noexcept {}

            void Derivatives:: UnderflowException()
            {
                throw Specific::Exception(CallSign,"Step Underflow");
            }


            void Derivatives:: SingularFunctionException()
            {
                throw Specific::Exception(CallSign,"Singular Function");
            }

            void Derivatives:: OutOfDomainException()
            {
                throw Specific::Exception(CallSign,"Value Out Of Definition Domain");
            }
        }

        
        //----------------------------------------------------------------------
        //
        //
        //! derivative code
        //
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Derivatives<T>:: Code : public Object
        {
        public:
            static const size_t                NTAB = 16;
            typedef Function<T,T>              FunctionType;
            typedef CxxArray<T,Memory::Dyadic> ArrayType;

            explicit Code() :
            Object(),
            zero(0),
            one(1),
            half(0.5),
            negativeHalf(-half),
            ctrl(1.4),
            ctrl2(ctrl*ctrl),
            ftol( Numeric<T>::SQRT_EPSILON ),
            //ftol( Numeric<T>::FTOL ),
            a(NTAB,NTAB)
            {
            }

            virtual ~Code() noexcept
            {
            }

            inline T ComputeLowerStep(const T x, const T h) const
            {
                volatile T temp = x - h;
                volatile T step = x-temp;;
                if(Fabs<T>::Of(step) <= zero) Kernel::Derivatives::UnderflowException();
                return step;
            }

            inline T ComputeUpperStep(const T x, const T h) const
            {
                volatile T temp = x + h;
                volatile T step = temp-x;
                if(Fabs<T>::Of(step) <= zero) Kernel::Derivatives::UnderflowException();
                return step;
            }

            inline T eval(FunctionType &F, const T x, T &h, const Interval<T> &I)
            {
                //--------------------------------------------------------------
                //
                // check
                //
                //--------------------------------------------------------------
                if(h<=0) Kernel::Derivatives::UnderflowException();

                //--------------------------------------------------------------
                //
                // initialize triplet
                //
                //--------------------------------------------------------------
                const        T delta     = half*h;
                const        T lowerStep = ComputeLowerStep(x,delta);
                const        T upperStep = ComputeUpperStep(x,delta);
                Triplet<T> xx = { x - lowerStep, x, x + upperStep };

                //--------------------------------------------------------------
                //
                // compute depending on triplet vs. interval
                //
                //--------------------------------------------------------------
                if( I.tryPack(xx) )
                {
                    // direct version
                    h = xx.c-xx.a;
                    return (F(xx.c) - F(xx.a))/h;
                }
                else
                {
                    // adaptive version
                    h = xx.c-xx.a;
                    xx.b = Clamp(xx.a,half*(xx.a+xx.c),xx.c);
                    const T Fa    = F(xx.a);
                    const T Fb    = F(xx.b);
                    const T Fc    = F(xx.c);
                    const T sigma = (Fc -Fa);
                    const T gamma = Twice( (Fc-Fb) + (Fa-Fb) );
                    const T u     = Clamp(negativeHalf,(x-xx.b)/h, half);

#if 0
                    {
                        static unsigned  id = 0;
                        const String     name = FormatString("adapt%u.dat",++id);
                        Libc::OutputFile fp(name);
                        const size_t np = 100;
                        for(size_t i=0;i<np;++i)
                        {
                            const T xt = xx.a + (i*h)/np;
                            const T ft = F(xt);
                            const T ut = xt-xx.b;
                            fp("%g %g %g\n",double(xt),double(ft), double(Fb + sigma * ut/h + gamma * Squared(ut/h)) );
                        }
                    }
#endif
                    return (sigma + Twice( gamma * u) )/h;
                }
            }


            inline T compute(FunctionType &F, const T x, T h, const Interval<T> &I, T &err)
            {
                T    ans   = a[1][1] = eval(F,x,h,I);
                bool first = true;
                for(size_t i=2,im=1;i<=NTAB;++i,++im)
                {
                    h /= ctrl;
                    a[1][i] = eval(F,x,h,I);
                    T fac = ctrl2;
                    for(size_t j=2,jm=1;j<=i;++j,++jm)
                    {
                        const T anst = a[j][i] = (a[jm][i]*fac-a[jm][im])/(fac-one);
                        const T errt = Max(Fabs<T>::Of(anst-a[jm][i]),Fabs<T>::Of(anst-a[jm][im]));
                        fac *= ctrl2;
                        if(first)
                        {
                            ans   = anst;
                            err   = errt;
                            first = false;
                        }
                        else
                        {
                            if(errt<err)
                            {
                                ans = anst;
                                err = errt;
                            }
                        }
                    }

                    if(Fabs<T>::Of(a[i][i]-a[im][im]) >= Twice(err) )
                        break;

                }
                return ans;

            }

            inline T compute(FunctionType &F, const T x, T h, const Interval<T> &I)
            {

                T       currErr    = 0;
                T       currRes    = compute(F,x,h,I,currErr);
            AUTHORIZED:
                const T authorized = ftol * Fabs<T>::Of(currRes);
                if(currErr<=authorized)
                    return currRes;
                h *= half;
                {
                    T       tempErr = 0;
                    const T tempRes = compute(F,x,h,I,tempErr);
                    
                    if(tempErr>currErr)
                    {
                        return currRes;
                    }
                    currErr = tempErr;
                    currRes = tempRes;
                    goto AUTHORIZED;
                }
                
            }

            const T   zero;
            const T   one;
            const T   half;
            const T   negativeHalf;
            const T   ctrl;
            const T   ctrl2;
            const T   ftol;
            Matrix<T> a;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


#define real_t float
#include "interface.hxx"

#undef  real_t
#define real_t double
#include "interface.hxx"

#undef  real_t
#define real_t long double
#include "interface.hxx"
    }
}
