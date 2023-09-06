#include "y/mkl/derivatives/interface.hpp"
#include "y/mkl/interpolation/polynomial.hpp"

#include "y/container/matrix.hpp"
#include "y/mkl/api.hpp"
#include "y/mkl/triplet.hpp"

#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
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

        template <typename T>
        class Derivatives<T>:: Code : public Object
        {
        public:
            typedef Function<T,T>              FunctionType;
            typedef CxxArray<T,Memory::Dyadic> ArrayType;
            typedef Vector<T,Memory::Dyadic>   VectorType;

            explicit Code() :
            Object(),
            xa(),
            ya(),
            mu(3,3),
            lu(3),
            cf(3),
            zp(16)
            {
            }

            virtual ~Code() noexcept
            {
            }

            //------------------------------------------------------------------
            //
            //
            //! evaluation from (x.b,F.b) on x.a <= x.b <= x.c
            //
            //
            //------------------------------------------------------------------
            inline T eval(FunctionType &F, const Triplet<T> &x, const T Fb)
            {
                static const T half(0.5);
                static const T zero(0.0);
                static const T one(1.0);
                static const T four(4.0);

                assert(x.isIncreasing());

                const T xm = half*(x.a+x.c);
                if(Fabs<T>::Of(xm-x.a) <= zero ) Kernel:: Derivatives:: UnderflowException();
                if(Fabs<T>::Of(xm-x.c) <= zero ) Kernel:: Derivatives:: UnderflowException();

                const T xx[4] = { x.a-x.b, zero, x.c-x.b, xm - x.b };
                const T ff[4] = { F(x.a),  Fb,   F(x.c),  F(xm)    };

                //--------------------------------------------------------------
                //
                // pre-computing:  pw[i][j] = xx[i]^j
                //
                //--------------------------------------------------------------
                T pw[4][5] = {
                    { one, xx[0], zero, zero, zero },
                    { one, xx[1], zero, zero, zero },
                    { one, xx[2], zero, zero, zero },
                    { one, xx[3], zero, zero, zero },
                };

                for(size_t i=0;i<4;++i)
                {
                    T      *xp = pw[i];
                    const T xi = xx[i];
                    for(size_t p=2;p<5;++p) xp[p] = ipower(xi,p);
                }

                //--------------------------------------------------------------
                //
                // compute rhs = sum_i x_i^k F_i
                //
                //--------------------------------------------------------------
                Antelope::Add<T> &xadd = lu.xadd();
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
                T sxp[5] = { four, zero, zero, zero, zero };
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
                if(!lu.build(mu))  Kernel::Derivatives::SingularFunctionException();
                lu.solve(mu,cf);

#if 1
                {
                    static unsigned count = 0;
                    const String    fname = FormatString("drvs%u.dat",++count);
                    Libc::OutputFile fp(fname);
                    const T xmin = x.a;
                    const T xmax = x.c;
                    const size_t np   = 100;
                    for(size_t i=0;i<np;++i)
                    {
                        const T xtmp = xmin + T(i) * (xmax-xmin)/np;
                        const T ftmp = F(xtmp);
                        const T u    = xtmp - x.b;
                        fp("%g %g %g\n", double(xtmp), double(ftmp), double(cf[1]+cf[2]*u+cf[3]*u*u) );
                    }
                }
#endif

                return cf[2];
            }

            //------------------------------------------------------------------
            //
            //
            //! preparing triplet and upgrading length for derivative evaluation
            //
            //
            //------------------------------------------------------------------
            static inline void SetMetrics(Triplet<T> &x, const T x0, T &length, const Interval<T> &I)  
            {
                static const T half(0.5);

                assert(length>0);
                if(!I.contains(x0)) Kernel::Derivatives::OutOfDomainException();

                const T hlen = half * length;
                x.b = x0;
                x.a = x0-hlen;
                x.c = x0+hlen;

                switch(I.lower.type)
                {
                    case UnboundedLimit:
                        break;

                    case IncludingLimit:
                        if(x.a<I.lower.value)
                            x.a = I.lower.value;
                        break;

                    case ExcludingLimit:
                        while(x.a<=I.lower.value)
                            x.a = half*(x.a+x.b);
                        break;
                }

                switch(I.upper.type)
                {
                    case UnboundedLimit:
                        break;

                    case IncludingLimit:
                        if(x.c>I.upper.value)
                            x.c = I.upper.value;
                        break;

                    case ExcludingLimit:
                        while(x.c>=I.upper.value)
                            x.c = half*(x.b+x.c);
                        break;
                            
                }

            }

            inline T compute(FunctionType      &F,
                             const T            x0,
                             const T            h,
                             const Interval<T> &I)
            {
                xa.free();
                ya.free();

                // first call
                Triplet<T> X;
                T          L = h;
                SetMetrics(X,x0,L,I);
                const T    Scaling = L;
                const T    F0      = F(x0);
                std::cerr << "Scaling=" << L << std::endl;
                xa << 1;
                ya << eval(F,X,F0);

                std::cerr << xa << " -> " << ya << std::endl;

                L /= 1.4;
                SetMetrics(X,x0,L,I);
                xa << L/Scaling;
                ya << eval(F,X,F0);
                std::cerr << xa << " -> " << ya << std::endl;

                L /= 1.4;
                SetMetrics(X,x0,L,I);
                xa << L/Scaling;
                ya << eval(F,X,F0);
                std::cerr << xa << " -> " << ya << std::endl;



                
                return 0;
            }

            




            VectorType                 xa;
            VectorType                 ya;
            Matrix<T>                  mu;
            LU<T>                      lu;
            ArrayType                  cf;
            PolynomialInterpolation<T> zp;
            
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
