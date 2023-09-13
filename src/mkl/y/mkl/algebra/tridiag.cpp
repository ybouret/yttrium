
#include "y/mkl/algebra/tridiag.hpp"
#include "y/object.hpp"
#include "y/type/nullify.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class TriDiag<T> :: Code : public Object
        {
        public:
            typedef CxxArray<T,Memory::Dyadic> ArrayType;

            inline virtual ~Code() noexcept {}
            inline explicit Code(const size_t n) :
            Object(),
            size(n),
            a(n),
            b(n),
            c(n),
            gam(n),
            zero(0)
            {
                assert(n>0);
            }

            const size_t size;
            ArrayType    a,b,c,gam;
            const T      zero;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "tridiag.hxx"

#undef  real_t
#define real_t double
#include "tridiag.hxx"

#undef  real_t
#define real_t long double
#include "tridiag.hxx"

    }

}

