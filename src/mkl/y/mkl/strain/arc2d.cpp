
#include "y/mkl/strain/arc2d.hpp"
#include "y/mkl/strain/filter.hpp"
#include "y/type/nullify.hpp"
#include "y/object.hpp"
#include "y/mkl/xreal.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            const char * const Arc2DInfo:: CallSign = "Strain::Arc2D";

            Arc2DInfo:: Arc2DInfo() noexcept
            {
            }

            Arc2DInfo:: ~Arc2DInfo() noexcept
            {
            }

            template <typename T>
            class Arc2D<T>:: Code : public Object
            {
            public:
                typedef CxxArray<Vertex,Memory::Dyadic> Coef2D;

                inline explicit Code() :
                Object(),
                xf(),
                yf(),
                cf(SIZE)
                {}

                inline virtual ~Code() noexcept {}

                inline void free() noexcept
                {
                    xf.free();
                    yf.free();
                }

                Filter<T> xf;
                Filter<T> yf;
                Coef2D    cf;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

        }

    }

}

#define real_t float
#include "arc2d.hxx"
#undef real_t

#define real_t double
#include "arc2d.hxx"
#undef real_t

#define real_t long double
#include "arc2d.hxx"
#undef real_t

#define real_t XReal<float>
#include "arc2d.hxx"
#undef real_t

#define real_t XReal<double>
#include "arc2d.hxx"
#undef real_t

#define real_t XReal<long double>
#include "arc2d.hxx"
#undef real_t
