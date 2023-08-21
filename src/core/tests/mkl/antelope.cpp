
#include "../main.hpp"
#include "y/utest/run.hpp"
//#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/mkl/scalar.hpp"
#include "y/mkl/api.hpp"
#include "y/ordered/heap.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"
#include "y/memory/solitary/specimen.hpp"
#include "y/memory/solitary/workspace.hpp"

using namespace Yttrium;
using namespace MKL;

namespace Yttrium
{
    namespace MKL
    {

        namespace   Antelope
        {



            template <typename T>
            struct NeedUnit
            {
                enum { Flag = true };
            };

            template <>
            struct NeedUnit<apn>
            {
                enum { Flag = false };
            };

            template <>
            struct NeedUnit<apz>
            {
                enum { Flag = false };
            };

            template <>
            struct NeedUnit<apq>
            {
                enum { Flag = false };
            };


            template <typename T,bool>
            struct AddInterface;


            template <typename T>
            struct AddInterface<T,true>
            {
                //______________________________________________________________
                //
                //! alias to hold Fabs
                //______________________________________________________________
                typedef typename  ScalarFor<T>::Type Scalar;

                //______________________________________________________________
                //
                //
                //! Unit to hold value and Fabs(value)
                //
                //______________________________________________________________
                class Unit
                {
                public:
                    class Comparator
                    {
                    public:
                        inline  Comparator() noexcept {}
                        inline ~Comparator() noexcept {}
                        inline SignType operator()(const Unit &lhs, const Unit &rhs) const noexcept
                        { return Comparison::CxxDecreasing(lhs.absValue,rhs.absValue); }

                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(Comparator);
                    };

                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________

                    //! setup from given value
                    inline Unit(const T &args) noexcept:
                    usrValue(args), absValue( MKL::Fabs<T>::Of(usrValue) )
                    { }

                    //! cleanup
                    inline ~Unit() noexcept {}

                    //! copy
                    inline Unit(const Unit &other) noexcept:
                    usrValue(other.usrValue), absValue(other.absValue)
                    { }

                    //__________________________________________________________
                    //
                    // Methods
                    //__________________________________________________________

                    //! retrieve original value by cast
                    inline operator T() const noexcept
                    {
                        return usrValue;
                    }

                    //! display original value
                    inline friend std::ostream & operator<<(std::ostream &os, const Unit &self)
                    {
                        os << self.usrValue;
                        return os;
                    }

                    //! addition
                    inline friend Unit operator+(const Unit &lhs, const Unit &rhs) noexcept
                    {
                        const T sum = lhs.usrValue + rhs.usrValue;
                        return Unit(sum);
                    }

                    //__________________________________________________________
                    //
                    // Members
                    //__________________________________________________________
                    const T      usrValue; //!< user's value
                    const Scalar absValue; //!< Fabs(usrValue)

                private:
                    Y_DISABLE_ASSIGN(Unit);
                };

                template <typename RAW_BUFFER>
                class Code_   : public Heap<Unit,RAW_BUFFER,typename Unit::Comparator>
                {
                public:
                    typedef Heap<Unit,RAW_BUFFER,typename Unit::Comparator> HeapType;
                    using HeapType::size;
                    using HeapType::insert;
                    using HeapType::pull;

                protected:
                    inline explicit Code_() noexcept      : HeapType() {}
                    inline explicit Code_(const size_t n) : HeapType(n,AsCapacity) {}

                public:
                    inline virtual ~Code_() noexcept {}

                    inline Code_ & operator<<(const T args)
                    {
                        insert(args);
                        return *this;
                    }

                    inline T sum()
                    {
                        if(  size() > 0 )
                        {
                            while(size()>1)
                            {
                                const Unit lhs = pull();
                                const Unit rhs = pull();
                                const Unit tmp = lhs+rhs;
                                std::cerr << "(" << lhs << ")+(" << rhs << ")=" << tmp << std::endl;
                                insert(tmp);
                            }
                            return pull();
                        }
                        else
                        {
                            return T(0);
                        }

                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Code_);
                };

                template <typename ALLOCATOR>
                struct FlexibleBuffer
                {
                    typedef Core::FlexibleRawBuffer<Unit,ALLOCATOR> Type;
                };

                template <size_t N>
                struct CompiledBuffer
                {
                    typedef Core::CompiledRawBuffer<N,Unit> Type;
                };
                
                template <typename ALLOCATOR = Memory::Dyadic>
                class Code : public Code_<typename FlexibleBuffer<ALLOCATOR>::Type>
                {
                public:
                    typedef Code_<typename FlexibleBuffer<ALLOCATOR>::Type> CodeType;
                    explicit Code() noexcept : CodeType() {}
                    explicit Code(const size_t n) : CodeType(n) {}
                    virtual ~Code() noexcept {}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Code);
                };

                template <size_t N>
                class StaticCode : public Code_<typename CompiledBuffer<N>::Type>
                {
                public:
                    typedef Code_<typename CompiledBuffer<N>::Type> CodeType;

                    explicit StaticCode() noexcept : CodeType() {}
                    virtual ~StaticCode() noexcept {}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(StaticCode);
                };


            };
            

            template <typename T>
            struct AddInterface<T,false>
            {
                template <typename SOLITARY>
                class Code_
                {
                public:
                    inline explicit Code_() : job(), acc( job.build() ) {}
                    inline virtual ~Code_() noexcept {}


                private:
                    SOLITARY    job;
                    T          &acc;
                };

                template <typename ALLOCATOR>
                class Code : public Code_< Memory::Specimen<T,ALLOCATOR> >
                {
                public:
                    typedef Code_< Memory::Specimen<T,ALLOCATOR> > CodeType;
                    using CodeType::acc;

                    explicit Code() : CodeType() {}
                    explicit Code(const size_t) : CodeType() {}
                    virtual ~Code() noexcept {}

                    inline void make(size_t)  { acc=0; }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Code);
                };

                template <size_t N>
                class StaticCode : public Code_< Memory::Workspace<T> >
                {
                public:
                    typedef Code_< Memory::Workspace<T> > CodeType;

                    explicit StaticCode() : CodeType(), count(0), total(N) {}
                    virtual ~StaticCode() noexcept {}

                    

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(StaticCode);
                    size_t       count;
                    const size_t total;
                };


            };

            template <typename T>
            struct AddProxy
            {
                static const bool               UseUnit = NeedUnit<T>::Flag;
                typedef AddInterface<T,UseUnit> Interface;
            };

        }

    }
}

Y_UTEST(mkl_antelope)
{
    Random::Rand ran;

    MKL::Antelope::AddInterface<double,true>::Code<Memory::Dyadic> code, code2(13);

    code.make(5);
    code << 8 << -1 << 2 << 4;
    code.insert(1,5);
    std::cerr << code.sum() << std::endl;
    code.free();


    MKL::Antelope::AddInterface<double,true>::StaticCode<24> scode;

    scode.make(5);
    scode << 8 << -1 << 2 << 4;
    scode.insert(1,5);
    std::cerr << scode.sum() << std::endl;
    scode.free();


    MKL::Antelope::AddInterface<double,false>::StaticCode<24> f_scode;

}
Y_UDONE()
