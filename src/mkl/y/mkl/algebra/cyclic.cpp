
#include "y/mkl/algebra/cyclic.hpp"
#include "y/object.hpp"
#include "y/type/nullify.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/api.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {
        static inline size_t CheckCyclic(const size_t n)
        {
            if(n<=2) throw Specific::Exception("Cyclic","size=%u too small",unsigned(n));
            return n;
        }

        template <typename T>
        class Cyclic<T>:: Code : public Object
        {
        public:
            typedef CxxArray<T,Memory::Dyadic>  ArrayType;
            typedef typename ScalarFor<T>::Type ScalarType;


            virtual ~Code() noexcept {}

            explicit Code(const size_t n) :
            size(n),
            bb(n),
            u(n),
            z(n),
            one(1)
            {
            }

            bool solve(Writable<T>       &x,
                       const Readable<T> &r,
                       TriDiag<T>        &td)
            {
                assert(size==x.size());
                assert(size==r.size());
                assert(size==td.size);

                const size_t       n     = size;
                const Readable<T> &b     = td.b;
                const T &          alpha = td.c[n];
                const T &          beta  = td.a[1];

                T gamma = -b[1];
                bb[1]   = b[1]-gamma;
                bb[n]   = b[n]-alpha*beta/gamma;
                for(size_t i=2;i<n;++i)
                    bb[i]=b[i];

                if(!td.solve(x,r,bb))
                    return false;

                u[1]=gamma;
                u[n]=alpha;
                for(size_t i=2;i<n;i++) u[i]=0;

                if(!td.solve(z,u,bb))
                    return false;

                const T fact=(x[1]+beta*x[n]/gamma)/(one+z[1]+beta*z[n]/gamma);

                for (size_t i=1;i<=n;i++) x[i] -= fact*z[i];

                return true;
            }

            const size_t size;
            ArrayType    bb,u,z;
            const T      one;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "cyclic.hxx"

#undef  real_t
#define real_t double
#include "cyclic.hxx"

#undef  real_t
#define real_t long double
#include "cyclic.hxx"

#undef  real_t
#define real_t XReal<float>
#include "cyclic.hxx"

#undef  real_t
#define real_t XReal<double>
#include "cyclic.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "cyclic.hxx"


#undef  real_t
#define real_t Complex<float>
#include "cyclic.hxx"

#undef  real_t
#define real_t Complex<double>
#include "cyclic.hxx"

#undef  real_t
#define real_t Complex<long double>
#include "cyclic.hxx"

#undef  real_t
#define real_t Complex< XReal<float> >
#include "cyclic.hxx"

#undef  real_t
#define real_t Complex< XReal<double> >
#include "cyclic.hxx"

#undef  real_t
#define real_t Complex< XReal<long double> >
#include "cyclic.hxx"


#undef  real_t
#define real_t apq
#include "cyclic.hxx"
    }

}


