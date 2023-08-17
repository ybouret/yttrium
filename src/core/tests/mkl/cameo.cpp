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

            template <typename UNITS> static inline
            T Reduce(UNITS &units)
            {
                assert(units.size()>1);
                std::cerr << units << std::endl;
                while(units.size()>1)
                {
                    const T lhs = units.pull();
                    const T rhs = units.pull();
                    const T sum = lhs+rhs;
                    units.push(sum);
                    std::cerr << units << std::endl;
                }
                assert(1==units.size());
                return units.pull();
            }


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

            template <typename UNITS> static inline
            T Reduce(UNITS &units)
            {
                assert(units.size()>1);
                T sum = units[1];
                for(size_t i=units.size();i>1;--i) sum += units[i];
                units.free();
                return sum;
            }

        };


        template <typename T>
        struct Proxy
        {
            typedef typename MKL::ScalarFor<T>::Type Scalar;

            class Unit
            {
            public:
                const T      usrValue;
                const Scalar absValue;
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
                    os << self.usrValue; //<< '@' << '|' << self.absValue << '|';
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

            typedef const Unit       UnitArgs;
            typedef HeapPolicy<Unit> Policy;

            template <typename ALLOCATOR>
            struct FlexibleUnits {
                typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type;
            };

            template <size_t N>
            struct CompiledUnits {
                typedef typename Policy:: template CompiledUnits<N>::Type Type;
            };


        };

        template <>
        struct Proxy<apq> {
            typedef apq Unit;

            typedef const Unit &     UnitArgs;
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



        template <
        typename  T,
        typename  PROXY,
        typename  STORAGE>
        class Addition : public STORAGE
        {
        public:
            Y_ARGS_DECL(T,Type);
            typedef typename PROXY::Unit     Unit;
            typedef typename PROXY::UnitArgs UnitArgs;

            explicit Addition() noexcept : STORAGE() {}
            virtual ~Addition() noexcept {}

            inline void push(ParamType args)
            {
                UnitArgs u = args;
                this->insert(u);
            }

            inline void push(const size_t n, ParamType args)
            {
                UnitArgs u = args;
                this->insert(u,n);
            }
            
            Type sum()
            {
                if(this->size()>0)
                    return PROXY::Policy::Reduce(*this);
                else
                    return 0;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Addition);
        };

        template <typename T, typename ALLOCATOR = Memory::Dyadic>
        class Add :
        public Addition<T,Proxy<T>,typename Proxy<T>::template FlexibleUnits<ALLOCATOR>::Type>
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Add);
        };



    }

}

Y_UTEST(mkl_cameo)
{
    Random::Rand ran;

#if 1
    {
        Cameo::Addition
        <
        double,
        Cameo::Proxy<double>,
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
        Cameo::Proxy<apq>,
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


#if 1
    {
        Cameo::Addition< Complex<float>,
        Cameo::Proxy< Complex<float> >,
        Cameo::Proxy< Complex<float> >::CompiledUnits<10>::Type
        >
        xadd;

        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );

        std::cerr << xadd.sum() << std::endl;

        std::cerr << xadd.sum() << std::endl;

    }
#endif



}
Y_UDONE()

