#include "y/mkl/derivatives/interface.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/container/cxx-array.hpp"
#include "y/calculus/ipower.hpp"

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
                throw Libc::Exception(ERANGE,"underflow step");
            }
        }

        template <typename T>
        class Derivatives<T>:: Code
        {
        public:
            static const size_t NTAB = 16;
            typedef Function<T,T>              FunctionType;
            typedef CxxArray<T,Memory::Dyadic> ArrayType;

            explicit Code() :
            a(NTAB,NTAB),
            mu(4,4),
            lu(4),
            xadd( lu.xadd() ),
            rhs(4)
            {
            }

            virtual ~Code() noexcept
            {
            }

            static inline T CheckState(const T x, const T h)
            {
                assert(h>0);
                static const T zero(0);
                volatile     T temp = x + h;
                const        T hd   = temp - x;
                if( Fabs<T>::Of(hd) <= zero ) Kernel::Derivatives:: UnderflowException();
                return hd;
            }



            T eval(FunctionType &F, const T x, const T Fx, const T delta, const T length)
            {
                static const T half(0.5);
                static const T zero(0);
                static const T one(1);

                volatile T lowerOffset  = -delta;
                volatile T upperOffset  = length-delta;
                volatile T centerOffset = half*(lowerOffset + upperOffset);

                T ff[4] = { Fx,   zero, zero, zero };
                T xx[4] = { zero, zero, zero, zero };

                T pw[4][5] = {
                    { one, zero, zero, zero, zero },
                    { one, zero, zero, zero, zero },
                    { one, zero, zero, zero, zero },
                };


                {
                    volatile T center = x + centerOffset;
                    volatile T lower  = x + lowerOffset;   if(Fabs<T>::Of(center-lower) <= zero) Kernel:: Derivatives:: UnderflowException();
                    volatile T upper  = x + upperOffset;   if(Fabs<T>::Of(upper-center) <= zero) Kernel:: Derivatives:: UnderflowException();

                    xx[1] = lowerOffset;  ff[1] = F(lower);
                    xx[2] = centerOffset; ff[2] = F(center);
                    xx[3] = upperOffset;  ff[3] = F(upper);
                }

                for(size_t i=0;i<4;++i)
                {
                    const T xi = xx[i];
                    pw[i][1] = xi;
                    for(size_t p=2;p<4;++p) pw[i][p] = ipower(xi,p);
                }
                


                return 0;
            }




            Matrix<T> a;
            Matrix<T> mu;
            LU<T>     lu;
            Antelope::Add<T> &xadd;
            ArrayType rhs;

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
