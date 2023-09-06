#include "y/mkl/interpolation/polynomial.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/container/cxx-array.hpp"
#include "y/object.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class PolynomialInterpolation<T>::Code : public Object
        {
        public:
            typedef CxxArray<T,Memory::Dyadic> ArrayType;

            inline virtual ~Code() noexcept {}

            inline explicit Code(const size_t n) :
            Object(), c(n), d(n), xadd(n)
            {

            }

            inline T compute(const T            x,
                             const Readable<T> &xa,
                             const Readable<T> &ya,
                             T                 &dy)
            {
                static const T zero(0.0);
                assert(xa.size() == ya.size());
                assert(c.size()  == d.size() );
                assert(xa.size() <= c.size() );

                const size_t n = xa.size();
                size_t       ns  = 1;
                {
                    T dif = Fabs<T>::Of(x-xa[1]);
                    for(size_t i=1;i<=n;++i) {
                        const T dift = Fabs<T>::Of(x-xa[i]);
                        if( dift < dif)
                        {
                            ns  = i;
                            dif = dift;
                        }
                        c[i]=ya[i];
                        d[i]=ya[i];
                    }
                }

                xadd.free();
                xadd << (dy=ya[ns--]);
                for(size_t m=1;m<n;++m)
                {
                    const size_t nm = n-m;
                    for (size_t i=1;i<=nm;++i)
                    {
                        const T ho  = xa[i]-x;
                        const T hp  = xa[i+m]-x;
                        const T w   = c[i+1]-d[i];
                        T       den = ho-hp;
                        if( Fabs<T>::Of(den) <= zero)
                            throw Specific::Exception("Polynomial Interpolation","Undeflow in abscissae");
                        den=w/den;
                        d[i]=hp*den;
                        c[i]=ho*den;
                    }
                    xadd << (dy=( (ns<<1) < nm ? c[ns+1] : d[ns--]));
                }

                return xadd.sum();
            }


            ArrayType        c,d;
            Antelope::Add<T> xadd;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };



#define real_t float
#include "polynomial.hxx"

#undef  real_t
#define real_t double
#include "polynomial.hxx"

#undef  real_t
#define real_t long double
#include "polynomial.hxx"

#undef  real_t
#define real_t XReal<float>
#include "polynomial.hxx"

#undef  real_t
#define real_t XReal<double>
#include "polynomial.hxx"


#undef  real_t
#define real_t XReal<long double>
#include "polynomial.hxx"

    }

}
