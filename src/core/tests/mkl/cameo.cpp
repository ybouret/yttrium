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


        template <typename T>
        struct HeapPolicy
        {
            class  Comparator
            {
            public:
                inline  Comparator() noexcept {}
                inline ~Comparator() noexcept {}


                inline SignType operator()(const T &lhs, const T &rhs)
                {
                    return Comparison::CxxIncreasing(lhs.absValue,rhs.absValue);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Comparator);
            };

            template <typename ALLOCATOR>
            struct FlexibleUnits
            {
                typedef Heap<T,Core::FlexibleRawBuffer<T,ALLOCATOR>,Comparator > Type;
            };

            template <size_t N>
            struct CompiledUnits
            {
                typedef Heap<T,Core::CompiledRawBuffer<N,T>,Comparator> Type;
            };
        };

        template <typename T>
        struct DirePolicy
        {
            template <typename ALLOCATOR>
            struct FlexibleUnits
            {
                typedef Dire<T,Core::FlexibleRawBuffer<T,ALLOCATOR> > Type;
            };

            template <size_t N>
            struct CompiledUnits
            {
                typedef Dire<T,Core::CompiledRawBuffer<N,T> > Type;
            };
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

            typedef HeapPolicy<Unit> Policy;

            template <typename ALLOCATOR>
            struct FlexibleUnits {
                typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type;
            };

            template <size_t N>
            struct CompiledUnits
            {
                typedef typename Policy:: template CompiledUnits<N>::Type Type;
            };


        };

        template <>
        struct Proxy<apq> {
            typedef apq Unit;

            typedef DirePolicy<Unit> Policy;

            template <typename ALLOCATOR>
            struct FlexibleUnits {
                typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type;
            };

            template <size_t N>
            struct CompiledUnits
            {
                typedef typename Policy:: template CompiledUnits<N>::Type Type;
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

