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
            typedef Function<T,T>              FunctionType;
            typedef CxxArray<T,Memory::Dyadic> ArrayType;
            typedef Vector<T,Memory::Dyadic>   VectorType;

            explicit Code() :
            Object(),
            xa(),
            ya(),
            mu(3,3),
            lu(3),
            cf(3)
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
            inline T adjusted(FunctionType &F, const Triplet<T> &x, const T Fb)
            {
                static const T half(0.5);
                static const T zero(0.0);
                static const T one(1.0);
                static const T four(4.0);


                assert(x.isIncreasing());

                const T xm = half*(x.a+x.c);
                const T xx[4] = { x.a-x.b, zero, x.c-x.b, xm - x.b };
                const T ff[4] = { F(x.a),  Fb,   F(x.c),  F(xm)    };

                {
                    static unsigned nadj = 1;
                    const String    name = FormatString("adj%u.dat",nadj++);
                    Libc::OutputFile fp(name);
                    for(size_t i=0;i<4;++i)
                    {
                        fp("%.15g %.15g\n", double(xx[i]+x.b), double(ff[i]));
                    }
                }


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
                        fp("%g %g %g %g\n", double(xtmp), double(ftmp), double(cf[1]+cf[2]*u+cf[3]*u*u), double(cf[1]+cf[2]*u) );
                    }
                }
#endif

                Y_DRVS("slope: " << cf[2]);
                return cf[2];
            }


            //------------------------------------------------------------------
            //
            //
            //! build x.a <= x.b=x0 <= x.c, try to conserve max length
            //
            //
            //------------------------------------------------------------------
            static inline bool StandardMetrics(Triplet<T> &x, const T x0, T &length, const Interval<T> &I)
            {
                static const T half(0.5);
                static const T zero(0);

                //--------------------------------------------------------------
                // check x0 is inside interval
                //--------------------------------------------------------------
                assert(length>0);
                if(!I.contains(x0)) Kernel::Derivatives::OutOfDomainException();

                //--------------------------------------------------------------
                //
                // prepare optimistic scenario
                //
                //--------------------------------------------------------------
                bool    result = true;
                {
                    const T hlen = length*half;
                    x.b = x0;
                    x.a = x0-hlen; if(Fabs<T>::Of(x.b-x.a) <= zero ) Kernel:: Derivatives:: UnderflowException();
                    x.c = x0+hlen; if(Fabs<T>::Of(x.c-x.b) <= zero ) Kernel:: Derivatives:: UnderflowException();
                }

                assert(x.isIncreasing());

                bool movedUpper = false; // did we already move upper value ?
                bool movedLower = false; // did we already move lower value ?

            PROBE:
                //--------------------------------------------------------------
                // check/increase lower bound
                //--------------------------------------------------------------
                switch(I.lower.type)
                {
                    case UnboundedLimit:
                        break;

                    case IncludingLimit:
                        if(x.a<I.lower.value)
                        {
                            movedLower = true;
                            result     = false;
                            Y_DRVS(" [incl] moving lower from " << x.a << " to " << I.lower.value);
                            if(!movedUpper)
                            {
                                Y_DRVS(" [incl] shifting upper value...");
                                x.c += (I.lower.value - x.a); // shift upper value
                            }
                            x.a  = I.lower.value;             // and set new lower value
                        }
                        break;

                    case ExcludingLimit:
                        if(x.a<=I.lower.value)
                        {
                            movedLower = true;
                            result     = false;
                            const T x_old = x.a;
                            do
                            {
                                x.a = half*(x.a+x.b); // make lower bound closer to central value
                            }
                            while(x.a<=I.lower.value);
                            Y_DRVS(" [excl] moving lower from " << x_old << " to " << x.a);
                            if(!movedUpper)
                            {
                                Y_DRVS(" [excl] shifting upper value...");
                                x.c += x.a - x_old;   // shift upper value
                            }

                        }
                        break;
                }

                //--------------------------------------------------------------
                // check/decrease upper bound
                //--------------------------------------------------------------
                switch(I.upper.type)
                {
                    case UnboundedLimit:
                        break;

                    case IncludingLimit:
                        if(x.c>I.upper.value)
                        {
                            result     = false;
                            movedUpper = true;
                            Y_DRVS(" [incl] moving upper value from " << x.c << " to " << I.upper.value);
                            if(movedLower)
                                x.c = I.upper.value; // set new upper value
                            else
                            {
                                Y_DRVS(" [incl] shifting lower value...");
                                x.a -= (x.c - I.upper.value); // shift lower value
                                x.c  = I.upper.value;         // set upper value
                                goto PROBE;                   // and check again
                            }
                        }
                        break;

                    case ExcludingLimit:
                        if(x.c>=I.upper.value)
                        {
                            result     = false;
                            movedUpper = true;
                            const T x_old = x.c;
                            do
                            {
                                // make upper value closer to central value
                                x.c = half*(x.b+x.c);
                            } while(x.c>=I.upper.value);
                            Y_DRVS(" [incl] moving upper value from " << x_old << " to " << x.c);

                            if(!movedLower)
                            {
                                // shift lower value and check again
                                Y_DRVS(" [incl] shifting lower value...");
                                x.a -= (x_old - x.c);
                                goto PROBE;
                            }
                        }
                        break;
                }

                //--------------------------------------------------------------
                // update length upon change
                //--------------------------------------------------------------
                if(!result)
                    length = x.c-x.a;

                return result;
            }

            inline T evaluate(FunctionType      &F,
                              const T            x,
                              T                 &h,
                              const Interval<T> &I,
                              bool              &hasFx,
                              T                 &Fx)
            {
                Triplet<T> xx = {x,x,x};
                Y_DRVS("evaluate h=" <<h);
                if( StandardMetrics(xx,x,h,I) )
                {
                    Y_DRVS("centered on " << xx);
                    return (F(xx.c) - F(xx.a))/h;
                }
                else
                {
                    if(!hasFx)
                    {
                        Fx    = F(x);
                        hasFx = true;
                    }
                    Y_DRVS("adjusted on " << xx);
                    return adjusted(F,xx,Fx);
                }
            }


            inline T compute(FunctionType      &F,
                             const T            x,
                             T                  h,
                             const Interval<T> &I)
            {

                static const T zero(0);
                static const T one(1);

                assert(h>0);

                // initialize
                bool hasFx = false;
                T       Fx = zero;
                xa.free();
                ya.free();

                ya << evaluate(F,x,h,I,hasFx,Fx);
                const T scaling = h;
                xa << one;

                for(size_t i=1;i<=5;++i)
                {
                    h /= 1.4;
                    ya << evaluate(F,x,h, I, hasFx, Fx);
                    xa << h/scaling;
                }

                {
                    Libc::OutputFile fp("drvs.dat");
                    for(size_t i=1;i<=xa.size();++i)
                    {
                        fp("%.15g %.15g\n", double(xa[i]), double(ya[i]));
                    }
                }

                

                return 0;
            }




            VectorType                 xa;
            VectorType                 ya;
            Matrix<T>                  mu;
            LU<T>                      lu;
            ArrayType                  cf;

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
