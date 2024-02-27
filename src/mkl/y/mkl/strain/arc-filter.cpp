
#include "y/mkl/strain/arc-filter.hpp"
#include "y/mkl/strain/filter.hpp"
#include "y/type/nullify.hpp"
#include "y/object.hpp"
#include "y/mkl/xreal.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            template <typename T>
            class ArcFilter<T>:: Code : public Object
            {
            public:

                inline explicit Code() : 
                Object(),
                xf(),
                yf()
                {}

                inline virtual ~Code() noexcept {}

                inline void free() noexcept
                {
                    xf.free();
                    yf.free();
                }

                Filter<T> xf;
                Filter<T> yf;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

        }

    }

}

#define real_t float
#include "arc-filter.hxx"
#undef real_t

#define real_t double
#include "arc-filter.hxx"
#undef real_t

#define real_t long double
#include "arc-filter.hxx"
#undef real_t

#define real_t XReal<float>
#include "arc-filter.hxx"
#undef real_t

#define real_t XReal<double>
#include "arc-filter.hxx"
#undef real_t

#define real_t XReal<long double>
#include "arc-filter.hxx"
#undef real_t
