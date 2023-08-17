#include "../main.hpp"

#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"

#include "y/ordered/heap.hpp"
#include "y/ordered/dire.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Cameo
    {


        template <typename T>
        struct HeapPolicy
        {
            class  Comparator
            {
            public:
                inline  Comparator() noexcept {}
                inline ~Comparator() noexcept {}

                inline SignType operator()(const T &lhs, const T &rhs) const
                {
                    return Comparison::CxxDecreasing(lhs.absValue,rhs.absValue);
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
                inline Unit(const T &args) noexcept:
                usrValue(args),
                absValue( MKL::Fabs<T>::Of(usrValue) )
                {
                }

                inline ~Unit() noexcept {}

                inline Unit(const Unit &other) noexcept:
                usrValue(other.usrValue),
                absValue(other.absValue)
                {
                }

                inline operator T() const noexcept
                {
                    return usrValue;
                }

                inline friend std::ostream & operator<<(std::ostream &os, const Unit &self)
                {
                    os << self.usrValue;
                    return os;
                }

                inline friend Unit operator+(const Unit &lhs, const Unit &rhs) noexcept
                {
                    const T sum = lhs.usrValue + rhs.usrValue;
                    return Unit(sum);
                }

            private:
                Y_DISABLE_ASSIGN(Unit);
            };

            typedef const Unit UnitArgs;

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

            typedef const Unit &UnitArgs;
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

#if 0
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

            typedef const Unit       UnitArgs;
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
#endif


        template <
        typename  T,
        typename  UNIT,
        typename  UNIT_ARGS,
        typename  STORAGE>
        class Addition : public STORAGE
        {
        public:
            Y_ARGS_DECL(T,Type);

            explicit Addition() noexcept : STORAGE() {}
            virtual ~Addition() noexcept {}

            inline void push(ParamType args)
            {
                UNIT_ARGS u = args;
                this->insert(u);
            }

            inline void push(const size_t n, ParamType args)
            {
                UNIT_ARGS u = args;
                this->insert(u,n);
            }
            
            Type sum()
            {
                if(this->size()>0)
                {
                    std::cerr << *this << std::endl;
                    while( this->size() > 1)
                    {
                        const UNIT lhs = this->pull();
                        const UNIT rhs = this->pull();
                        const UNIT tmp = lhs+rhs;
                        this->insert(tmp);
                        std::cerr << *this << std::endl;
                    }
                    return this->pull();
                }
                else
                {
                    return 0;
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Addition);
        };


    }

}

Y_UTEST(mkl_cameo)
{

#if 0
    {
        Cameo::Addition
        <
        double,
        Cameo::Proxy<double>::Unit,
        Cameo::Proxy<double>::UnitArgs,
        Cameo::Proxy<double>::FlexibleUnits<Memory::Pooled>::Type
        > xadd;

        xadd.push(10);
        xadd.push(2,3.1);
        xadd.push(100);
        xadd.push(-4);
        xadd.push(-50.3);

        std::cerr << xadd << std::endl;

        const double res = xadd.sum();
        std::cerr << "res=" << res << std::endl;
    }
#endif

#if 1
    {
        Cameo::Addition<apq,
        Cameo::Proxy<apq>::Unit,
        Cameo::Proxy<apq>::UnitArgs,
        Cameo::Proxy<apq>::FlexibleUnits<Memory::Dyadic>::Type
        >
        xadd;

        std::cerr << xadd.sum() << std::endl;

        xadd.push( apq(1,2)  );
        xadd.push( apq(-1,3) );
        xadd.push( apq(1)    );
        std::cerr << xadd.sum() << std::endl;

    }
#endif

}
Y_UDONE()

