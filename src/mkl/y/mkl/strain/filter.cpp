#include "y/mkl/strain/filter.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/v2d.hpp"
#include "y/sequence/vector.hpp"

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
                typedef V2D<T>                       Point;
                typedef Vector<Point,Memory::Dyadic> Points;
                typedef Antelope::Add<T>             XAdd;

                inline explicit Code() : points(), solver(), xadd() {}
                inline virtual ~Code() noexcept {}



                Points points;
                LU<T>  solver;
                XAdd   xadd;

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
