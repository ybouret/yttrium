
#include "../main.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/mkl/antelope/wary.hpp"

#include "y/mkl/scalar.hpp"
#include "y/mkl/api.hpp"
#include "y/ordered/heap.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"

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


                    typedef Core::FlexibleRawBuffer<Unit,Memory::Dyadic> Buffer;

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


                class Code : public Heap<Unit,typename Unit::Buffer, typename Unit::Comparator>
                {
                public:
                    typedef Heap<Unit,typename Unit::Buffer, typename Unit::Comparator> HeapType;
                    using HeapType::size;
                    using HeapType::insert;
                    using HeapType::pull;

                    inline explicit Code()      noexcept : HeapType() {}
                    inline explicit Code(const size_t n) : HeapType(n,AsCapacity) {}
                    inline virtual ~Code() noexcept {}

                    inline T sum()
                    {
                        if(  size() > 0 )
                        {
                            while(size()>1)
                            {
                                const Unit lhs = pull();
                                const Unit rhs = pull();
                                const Unit tmp = lhs+rhs;
                                //std::cerr << "(" << lhs << ")+(" << rhs << ")=" << tmp << std::endl;
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
                    Y_DISABLE_COPY_AND_ASSIGN(Code);
                };


            };
            

            template <typename T>
            struct AddInterface<T,false>
            {

                class Code
                {
                public:
                    explicit Code()             : acc(0) {}
                    explicit Code(const size_t) : acc(0) {}
                    virtual ~Code() noexcept {}

                    inline void make(size_t) {
                        acc = 0;
                    }

                    inline void free()
                    {
                        acc = 0;
                    }

                    inline T sum()
                    {
                        T res = acc;
                        acc = 0;
                        return acc;
                    }

                    inline void insert(const T &args)
                    {
                        acc += args;
                    }

                private:
                    T acc;
                };

            };

            template <typename T>
            class Add : public AddInterface<T,NeedUnit<T>::Flag>::Code
            {
            public:
                typedef typename AddInterface<T,NeedUnit<T>::Flag>::Code CodeType;

                explicit Add() : CodeType() {}
                virtual ~Add() noexcept {}



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Add);
            };


        }

    }
}

#include "y/text/justify.hpp"

template <typename T>
static inline void ShowUnit( const char *name, Random::Bits &ran )
{
    std::cerr << '[' << Justify(name,32,Justify::Center) << ']' << std::endl;
    std::cerr << std::setw(16) << sizeof( typename MKL::Antelope::AddInterface<T,true>::Unit ) << std::endl;

    MKL::Antelope::Add<T> xadd;

    xadd.free();

    std::cerr << xadd.sum();

    for(size_t i=1+ran.leq(10);i>0;--i)
    {
        const T tmp = Bring<T>::Get(ran);
        std::cerr << "+(" << tmp << ")";
        xadd.insert( tmp );
    }
    std::cerr << " = " << xadd.sum() << std::endl;

}

#define Y_SHOW_UNIT(CLASS) ShowUnit<CLASS>( #CLASS, ran)



Y_UTEST(mkl_antelope)
{
    Random::Rand ran;

    Y_SHOW_UNIT(float);
    Y_SHOW_UNIT(double);
    Y_SHOW_UNIT(long double);

    Y_SHOW_UNIT(Complex<float>);
    Y_SHOW_UNIT(Complex<double>);
    Y_SHOW_UNIT(Complex<long double>);


    Y_SHOW_UNIT(XReal<float>);
    Y_SHOW_UNIT(XReal<double>);
    Y_SHOW_UNIT(XReal<long double>);
    Y_SHOW_UNIT(Complex< XReal<float> >);
    Y_SHOW_UNIT(Complex< XReal<double> >);
    Y_SHOW_UNIT(Complex< XReal<long double> >);




}
Y_UDONE()
