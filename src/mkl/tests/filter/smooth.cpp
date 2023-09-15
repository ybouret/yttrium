#include "y/mkl/filter/selector.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/random/bits.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/container/cxx/array.hpp"
#include "y/type/nullify.hpp"
#include "y/calculus/ipower.hpp"

#include <cmath>

using namespace Yttrium;


namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class Smooth
        {
        public:

            class Code
            {
            public:
                typedef CxxArray<T,Memory::Dyadic> ArrayType;

                explicit Code(const size_t degree) :
                dg(degree),
                nc(dg+1),
                mu(nc,nc),
                cf(nc),
                yOffset(0)
                {
                }

                virtual ~Code() noexcept
                {

                }

                template <typename SEQUENCE>
                void prolog(const T            xx,
                           SEQUENCE          &xs,
                           SEQUENCE          &ys,
                           Antelope::Add<T>  &xadd)
                {
                    const size_t n = xs.size();
                    yOffset = xadd.average(ys);
                    std::cerr << "yOffset=" << yOffset << std::endl;
                    ArrayType x(n), y(n);
                    for(size_t i=n;i>0;--i)
                    {
                        x[i] = xs[i] - xx;
                        y[i] = ys[i] - yOffset;
                    }

                    for(size_t d=0,k=1;d<=dg;++d,++k)
                    {
                        xadd.free();
                        for(size_t i=n;i>0;--i)
                        {
                            xadd << y[i] * ipower(x[i],d);
                        }
                        cf[k] = xadd.sum();
                    }
                    std::cerr << "rhs=" << cf << std::endl;
                    for(size_t r=1;r<=nc;++r)
                    {
                        for(size_t c=1;c<=nc;++c)
                        {
                            const size_t d = r+c-2;
                            xadd.free();
                            for(size_t i=n;i>0;--i)
                            {
                                xadd << ipower(x[i],d);
                            }
                            mu[r][c] = mu[c][r] = xadd.sum();
                        }
                    }
                    std::cerr << "mu=" << mu << std::endl;
                }

                inline void epilog()
                {
                    cf[1] += yOffset;
                    for(size_t i=3;i<=nc;++i)
                    {
                        cf[i] *= T(i-1);
                    }
                }

                const size_t   dg;
                const size_t   nc;
                Matrix<T>      mu;
                ArrayType      cf;
                T              yOffset;

            private:

            };

            inline explicit Smooth() : code(0)
            {
            }

            inline virtual ~Smooth()
            {
                Nullify(code);
            }

            template <typename SEQUENCE>
            bool prepare(const T   xx,
                         SEQUENCE &xs,
                         SEQUENCE &ys,
                         const size_t order)
            {
                assert(xs.size()==ys.size());
                assert(xs.size()>0);
                const size_t np = xs.size();
                const size_t dg = Min(order,np-1);
                if(0==code||code->dg!=dg)
                {
                    Nullify(code);
                    code = new Code(dg);
                }
                code->prolog(xx,xs,ys,xadd);
                if(!lu.build(code->mu)) return false;
                lu.solve(code->mu,code->cf);
                code->epilog();
                std::cerr << "cf=" << code->cf << std::endl;
                return true;
            }

            size_t degree() const noexcept
            {
                assert(0!=code);
                return code->dg;
            }

            const T operator[](const size_t dg) const noexcept
            {
                assert(0!=code);
                const size_t ic = dg+1;
                if(ic>code->cf.size())
                    return T(0);
                else
                    return code->cf[ic];
            }

            Code            *code;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Smooth);
            Antelope::Add<T> xadd;
            MKL::LU<T>       lu;
        };

    }
}

Y_UTEST(filter_smooth)
{

    Random::Rand ran;

    Vector<double> X,Y;


    {
        const size_t np=100;
        Libc::OutputFile fp("smooth-data.dat");
        double xx = 0;
        for(size_t i=0;i<=np;++i)
        {
            xx += 0.05 + 0.05 * ran.to<double>();
            X << xx;
            Y << cos(xx);
            fp("%.15g %.15g\n", X.tail(), Y.tail());
        }

    }

    const double   xmin  = 0;
    const double   xmax  = X.tail()*1.01;
    Vector<double> xs, ys;


    MKL::Antelope::Add<double> xadd;

    MKL::Smooth<double> sm;

    {
        Libc::OutputFile fp("smooth.dat");
        const size_t NP = 10;
        for(size_t i=0;i<=NP;++i)
        {
            const double xx = xmin + i*(xmax-xmin)/double(NP);
            std::cerr << "xx=" << xx << std::endl;
            MKL::Selector::Fill(xx, 0.5, 0.7, xs, ys, X, Y);




            if(!sm.prepare(xx, xs, ys,5)) throw Exception("singular smooth");

            if(false)
            {
                Libc::OutputFile fps("select.dat");
                for(size_t j=1;j<=xs.size();++j)
                {
                    fps("%.15g %.15g\n", xs[j], ys[j]);
                }
                fps << '\n';
                fps("%.15g %.15g\n", xx, sm.code->yOffset);
                fps("%.15g %.15g\n", xx, 0.0);

            }

            fp("%.15g %.15g\n", xx, sm[0] );
        }
    }








}
Y_UDONE()

