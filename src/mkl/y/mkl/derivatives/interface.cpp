#include "y/mkl/derivatives/interface.hpp"
#include "y/mkl/api.hpp"
#include "y/mkl/triplet.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/mkl/numeric.hpp"



#include "y/system/exception.hpp"
#include "y/container/cxx-array.hpp"
#include "y/calculus/ipower.hpp"
#include "y/sequence/vector.hpp"

#include "y/stream/libc/output.hpp"
#include "y/string.hpp"



namespace Yttrium
{
    namespace MKL
    {

        namespace Kernel
        {
            const char * const Derivatives:: CallSign = "Derivatives";
            bool               Derivatives:: Verbose  = false;

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

#define Y_DRVS(MSG) do { if(Kernel::Derivatives::Verbose) std::cerr << "[drvs] " << MSG << std::endl; } while(false)

        template <typename T>
        class Derivatives<T>:: Code : public Object
        {
        public:
            static const size_t                NTAB = 16;
            typedef Function<T,T>              FunctionType;
            typedef CxxArray<T,Memory::Dyadic> ArrayType;
            typedef Vector<T,Memory::Dyadic>   VectorType;

            explicit Code() :
            Object()

            {
            }

            virtual ~Code() noexcept
            {
            }

            static inline T ComputeLowerStep(const T x, const T h)
            {
                static const T zero(0);
                volatile T temp = x - h;
                volatile T step = x-temp;;
                if(Fabs<T>::Of(step) <= zero) Kernel::Derivatives::UnderflowException();
                return step;
            }

            static inline T ComputeUpperStep(const T x, const T h)
            {
                static const T zero(0);
                volatile T temp = x + h;
                volatile T step = temp-x;
                if(Fabs<T>::Of(step) <= zero) Kernel::Derivatives::UnderflowException();
                return step;
            }

            inline T eval(FunctionType &F, const T x, T &h, const Interval<T> &I)
            {
                // check
                if(h<=0) Kernel::Derivatives::UnderflowException();

                // initialize triplet
                static const T half(0.5);
                static const T negHalf   = -half;
                const        T delta     = half*h;
                const        T lowerStep = ComputeLowerStep(x,delta);
                const        T upperStep = ComputeUpperStep(x,delta);
                Triplet<T> xx = { x - lowerStep, x, x + upperStep };


                // compute depending on triplet vs. interval
                if( I.tryPack(xx) )
                {
                    // direct version
                    Y_DRVS("direct   on " << xx);
                    h = xx.c-xx.a;
                    return (F(xx.c) - F(xx.a))/h;
                }
                else
                {
                    // adaptive version
                    Y_DRVS("adaptive on " << xx);
                    h = xx.c-xx.a;
                    xx.b = Clamp(xx.a,half*(xx.a+xx.c),xx.c);
                    const T Fa    = F(xx.a);
                    const T Fb    = F(xx.b);
                    const T Fc    = F(xx.c);
                    const T sigma = (Fc -Fa);
                    const T gamma = Twice( (Fc-Fb) + (Fa-Fb) );
                    const T u     = Clamp(negHalf,(x-xx.b)/h, half);

#if 1
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

            inline T compute(FunctionType &F, const T x, T h, const Interval<T> &I)
            {
                Libc::OutputFile fp("drvs.dat");
                for(size_t i=1;i<=5;++i)
                {
                    T df = eval(F,x,h,I);
                    std::cerr << "h=" << h << " df=" << df << std::endl;

                    fp("%.15g %.15g\n",h,df);
                    h/=1.4;
                }


                return 0;
            }




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
