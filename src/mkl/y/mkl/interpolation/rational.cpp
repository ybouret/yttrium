
#include "y/mkl/interpolation/rational.hpp"
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
        class RationalInterpolation<T>::Code : public Object
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
                static const T zero(0);
                static const T TINY(1e-25);
                assert(xa.size() == ya.size());
                assert(c.size()  == d.size() );
                assert(xa.size() <= c.size() );

                const size_t n = xa.size();
                size_t       ns  = 1;

                T hh= Fabs<T>::Of(x-xa[1]);
                for(size_t i=1;i<=n;i++)
                {
                    const T h=Fabs<T>::Of(x-xa[i]);
                    if (h <= zero)
                    {
                        dy=0;
                        return ya[i];
                    }
                    else
                        if (h < hh)
                        {
                            ns=i;
                            hh=h;
                        }
                    c[i]=ya[i];
                    d[i]=ya[i]+TINY;
                }
                xadd.free();
                xadd << (dy=ya[ns--]);
                for(size_t m=1;m<n;m++)
                {
                    const size_t nm = n-m;
                    for (size_t i=1;i<=nm;i++)
                    {
                        const T w  = c[i+1]-d[i];
                        const T h  = xa[i+m]-x;
                        const T t  = (xa[i]-x)*d[i]/h;
                        T       dd = t-c[i+1];
                        if( Fabs<T>::Of(dd) <= zero)
                            throw Specific::Exception("Rational Interpolation","Underflow in abscissae");
                        dd=w/dd;
                        d[i]=c[i+1]*dd;
                        c[i]=t*dd;
                    }
                    //y += (dy=(2*ns < nm ? c[ns+1] : d[ns--]));
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
#include "rational.hxx"

#undef  real_t
#define real_t double
#include "rational.hxx"

#undef  real_t
#define real_t long double
#include "rational.hxx"

#undef  real_t
#define real_t XReal<float>
#include "rational.hxx"

#undef  real_t
#define real_t XReal<double>
#include "rational.hxx"


#undef  real_t
#define real_t XReal<long double>
#include "rational.hxx"

    }

}
