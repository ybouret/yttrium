#include "y/mkl/derivatives/interface.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/api.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class Derivatives<T>:: Code
        {
        public:
            static const size_t NTAB = Kernel::Derivatives::NTAB;

            explicit Code() :
            a(NTAB,NTAB)
            {
            }

            virtual ~Code() noexcept
            {
            }

            Matrix<T> a;

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
