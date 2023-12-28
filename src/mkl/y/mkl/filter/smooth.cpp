#include "y/mkl/filter/smooth.hpp"
#include "y/mkl/statistics/metrics.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/container/cxx/array.hpp"

#include "y/type/nullify.hpp"

#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/calculus/ipower.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class Smooth<T> :: Code : public Object
        {
        public:
            typedef Memory::Dyadic    Model;
            typedef Vector<T,Model>   Tableau;
            typedef Antelope::Add<T>  XAdd;
            typedef CxxArray<T,Model> Array;

            inline explicit Code() :
            zero(0),
            coef(4,zero),
            solv(),
            ttab(),
            ztab()
            {}

            inline virtual ~Code() noexcept {}

            inline void run(const T           &t0,
                            const Readable<T> &t,
                            const Readable<T> &z,
                            const size_t       degree)
            {
                assert(t.size()==z.size());

                //--------------------------------------------------------------
                // prepare resources
                //--------------------------------------------------------------
                coef.ld(zero);
                const size_t n = t.size(); if(n<=0) return;
                const size_t m = Min(degree+1,n);
                solv.ensure(m);
                ttab.adjust(n,zero);
                ztab.adjust(n,zero);
                XAdd &xadd = solv.xadd();
                xadd.make(n);           assert( xadd.isEmpty() );

                //--------------------------------------------------------------
                // center t
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    ttab[i] = t[i] - t0;
                    xadd << z[i];
                }

                //--------------------------------------------------------------
                // center z
                //--------------------------------------------------------------
                const T z0 = xadd.sum()/T(n);
                //std::cerr << "z0=" << z0 << std::endl;
                for(size_t i=n;i>0;--i)
                {
                    ztab[i] = z[i] - z0;
                }

                //--------------------------------------------------------------
                // compute coefficient
                //--------------------------------------------------------------
                Array     cf(m,zero);
                {
                    Matrix<T> mu(m,m);
                    for(size_t i=1;i<=m;++i)
                    {
                        for(size_t j=i;j<=m;++j)
                        {
                            const size_t p = j+i-2;
                            xadd.free();
                            for(size_t k=n;k>0;--k)
                            {
                                const T tmp = ipower(ttab[k],p);
                                xadd << tmp;
                            }
                            mu[i][j] = mu[j][i] = xadd.sum();
                        }
                    }
                    //std::cerr << "mu=" << mu << std::endl;
                    if(!solv.build(mu)) throw Libc::Exception(EDOM,"singular smoothing moments");
                    
                    for(size_t j=2;j<=m;++j)
                    {
                        xadd.free();
                        for(size_t k=n;k>0;--k)
                        {
                            const T tmp = ipower(ttab[k],j-1);
                            xadd << tmp * ztab[k];
                        }
                        cf[j] = xadd.sum();
                    }
                    //std::cerr << "rs=" << cf << std::endl;
                    solv.solve(mu,cf);
                }
                //std::cerr << "cf=" << cf << std::endl;

                {        coef[1] = cf[1] + z0; }
                if(m>=2) coef[2] = cf[2];
                if(m>=3) coef[3] = cf[3]+cf[3];

            }

            const T          zero;
            Array            coef;
            LU<T>            solv;
            Tableau          ttab;
            Tableau          ztab;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "smooth.hxx"
#undef real_t

#define real_t double
#include "smooth.hxx"
#undef real_t

#define real_t long double
#include "smooth.hxx"
#undef real_t

#define real_t XReal<float>
#include "smooth.hxx"
#undef real_t

#define real_t XReal<double>
#include "smooth.hxx"
#undef real_t

#define real_t XReal<long double>
#include "smooth.hxx"
#undef real_t

    }

}


