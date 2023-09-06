#include "y/mkl/derivatives/interface.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/container/cxx-array.hpp"
#include "y/calculus/ipower.hpp"

#include "y/stream/libc/output.hpp"

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
            mu(3,3),
            lu(3),
            xadd( lu.xadd() ),
            cf(3)
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

                //--------------------------------------------------------------
                //
                // computing offsets and associated values
                //
                //--------------------------------------------------------------
                volatile T lowerOffset  = -delta;
                volatile T upperOffset  = length-delta;
                volatile T centerOffset = half*(lowerOffset + upperOffset);

                T xx[4] = { zero, zero, zero, zero };
                T ff[4] = { Fx,   zero, zero, zero };

                {
                    volatile T center = x + centerOffset;
                    volatile T lower  = x + lowerOffset;   if(Fabs<T>::Of(center-lower) <= zero) Kernel:: Derivatives:: UnderflowException();
                    volatile T upper  = x + upperOffset;   if(Fabs<T>::Of(upper-center) <= zero) Kernel:: Derivatives:: UnderflowException();

                    xx[1] = lowerOffset;  ff[1] = F(lower);
                    xx[2] = centerOffset; ff[2] = F(center);
                    xx[3] = upperOffset;  ff[3] = F(upper);
                }

                //--------------------------------------------------------------
                //
                // pre-computing:  pw[i][j] = xx[i]^j
                //
                //--------------------------------------------------------------
                T pw[4][5] = {
                    { one, zero, zero, zero, zero },
                    { one, zero, zero, zero, zero },
                    { one, zero, zero, zero, zero },
                    { one, zero, zero, zero, zero },
                };

                for(size_t i=0;i<4;++i)
                {
                    T      *xp = pw[i];
                    const T xi = xx[i];
                    xp[1] = xi;
                    for(size_t p=2;p<5;++p) xp[p] = ipower(xi,p);
                }

                //--------------------------------------------------------------
                //
                // compute rhs = sum_i x_i^k F_i
                //
                //--------------------------------------------------------------
                for(size_t k=0;k<3;)
                {
                    xadd.free();
                    for(size_t i=0;i<4;++i)
                        xadd << pw[i][k] * ff[i];
                    cf[++k] = xadd.sum();
                }

                //--------------------------------------------------------------
                //
                // precomputing sxp[p] = sum_i x_i^p
                //
                //--------------------------------------------------------------
                T sxp[5] = { 4, zero, zero, zero, zero };
                for(size_t p=1;p<=4;++p)
                {
                    xadd.free();
                    for(size_t i=0;i<4;++i) xadd << pw[i][p];
                    sxp[p] = xadd.sum();
                }

                //--------------------------------------------------------------
                //
                // computing matrix of moments
                //
                //--------------------------------------------------------------
                for(size_t i=0,ii=1;i<3;++i,++ii)
                {
                    Writable<T> &mi = mu[ii];
                    for(size_t j=0,jj=1;j<3;++j,++jj)
                    {
                        mi[jj] = sxp[i+j];
                    }
                }

                //--------------------------------------------------------------
                //
                // solving
                //
                //--------------------------------------------------------------
                if(!lu.build(mu)) throw Specific::Exception(Kernel::Derivatives::CallSign,"singular function");
                lu.solve(mu,cf);
                std::cerr << "cf=" << cf << std::endl;

#if 0
                {
                    Libc::OutputFile fp("drvs.dat");
                    const T xmin = x+lowerOffset;
                    const T xmax = x+upperOffset;
                    const size_t np   = 100;
                    for(size_t i=0;i<np;++i)
                    {
                        const T xtmp = xmin + T(i) * (xmax-xmin)/np;
                        const T ftmp = F(xtmp);
                        const T u    = xtmp - x;
                        fp("%g %g %g\n", double(u), double(ftmp), double(cf[1]+cf[2]*u+cf[3]*u*u) );
                    }
                }
#endif

                return cf[2];
            }




            Matrix<T> a;
            Matrix<T> mu;
            LU<T>     lu;
            Antelope::Add<T> &xadd;
            ArrayType cf;

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
