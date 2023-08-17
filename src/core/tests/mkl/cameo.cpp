#include "../main.hpp"

#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"

#include "y/ordered/heap.hpp"
#include "y/ordered/dire.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace CamEO
    {

        template <typename T> struct HeapPolicy
        {
            
        };

        template <typename T>
        struct Proxy
        {
            class Unit
            {
            public:
                const T usrValue;
                const T absValue;
                inline Unit(const T &args) :
                usrValue(args),
                absValue( MKL::Fabs<T>::Of(usrValue) )
                {
                }

                inline ~Unit() noexcept {}

                inline Unit(const Unit &other) :
                usrValue(other.usrValue),
                absValue(other.absValue)
                {
                }

            private:
                Y_DISABLE_ASSIGN(Unit);
            };

            class Comparator
            {
            public:
                inline  Comparator() noexcept {}
                inline ~Comparator() noexcept {}

                inline SignType operator()(const Unit &lhs, const Unit &rhs)
                {
                    return Comparison::CxxIncreasing(lhs.absValue,rhs.absValue);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Comparator);
            };

            template <typename ALLOCATOR>
            struct FlexibleUnits
            {
                typedef Heap<Unit,Core::FlexibleRawBuffer<Unit,ALLOCATOR>,Comparator> Type;
            };

            template <size_t N>
            struct CompiledUnits
            {
                typedef Heap<Unit,Core::CompiledRawBuffer<N,Unit>,Comparator> Type;
            };


        };

        template <>
        struct Proxy<apq> {
            typedef apq Unit;

            template <typename ALLOCATOR>
            struct FlexibleUnits
            {
                typedef Dire<Unit,Core::FlexibleRawBuffer<Unit,ALLOCATOR> > Type;
            };

            template <size_t N>
            struct CompiledUnits
            {
                typedef Dire<Unit,Core::CompiledRawBuffer<N,Unit> > Type;
            };
        };

        template <typename T>
        struct Proxy< Complex<T> >
        {
            class Unit
            {
            public:
                const Complex<T> usrValue;
                const T          absValue;
                inline Unit(const T &args) :
                usrValue(args),
                absValue( MKL::Fabs< Complex<T> >::Of(usrValue) )
                {
                }

                inline ~Unit() noexcept {}

                inline Unit(const Unit &other) :
                usrValue(other.usrValue),
                absValue(other.absValue)
                {
                }

            private:
                Y_DISABLE_ASSIGN(Unit);
            };


        };


    }

}

Y_UTEST(mkl_cameo)
{

}
Y_UDONE()

