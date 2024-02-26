#include "y/mkl/strain/filter.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            template <typename T>
            class Filter<T>:: Code : public Object
            {
            public:

                inline explicit Code() {}
                inline virtual ~Code() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

        }
    }
}


#define real_t float
#include "filter.hxx"
#undef real_t

#define real_t double
#include "filter.hxx"
#undef real_t

#define real_t long double
#include "filter.hxx"
#undef real_t

#define real_t XReal<float>
#include "filter.hxx"
#undef real_t

#define real_t XReal<double>
#include "filter.hxx"
#undef real_t

#define real_t XReal<long double>
#include "filter.hxx"
#undef real_t
