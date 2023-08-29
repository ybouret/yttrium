
#include "y/mkl/lak/lu.hpp"
#include "y/mkl/api.hpp"
#include "y/type/utils.hpp"
#include "y/container/cxx-array.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class LU<T> :: Code : public Object
        {
        public:
            typedef T                           Type;   //!< hold values
            typedef typename ScalarFor<T>::Type Scalar; //!< hold absolute values

            explicit Code(const size_t nmin) :
            s0(0),
            s1(1),
            scal(nmin),
            indx(nmin)
            {}

            virtual ~Code() noexcept {}

            bool build(Matrix<T> &a)
            {
                assert(a.isSquare());
                assert(a.rows>0);
                assert(a.rows<=scal.size());
                assert(a.rows<=indx.size());
                const size_t n = a.size();

                // check scaling
                for(size_t i=n;i>0;--i)
                {
                    Scalar                piv = s0;
                    const Readable<Type> &a_i = a[i];
                    for(size_t j=n;j>0;--j)
                    {
                        const Scalar tmp = Fabs<Type>::Of(a_i[j]);
                        if(tmp>piv) piv = tmp;
                    }
                    if(piv<=s0) return false;
                    scal[i] = s1/piv;
                }


                return true;
            }

            const Scalar                    s0;
            const Scalar                    s1;
            CxxArray<Scalar,Memory::Dyadic> scal;
            CxxArray<size_t,Memory::Dyadic> indx;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "lu.hxx"

#undef  real_t
#define real_t double
#include "lu.hxx"

#undef  real_t
#define real_t long double
#include "lu.hxx"

#undef  real_t
#define real_t XReal<float>
#include "lu.hxx"

#undef  real_t
#define real_t XReal<double>
#include "lu.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "lu.hxx"

#undef  real_t
#define real_t Complex<float>
#include "lu.hxx"

#undef  real_t
#define real_t Complex<double>
#include "lu.hxx"

#undef  real_t
#define real_t Complex<long double>
#include "lu.hxx"


    }

}


