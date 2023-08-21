
#include "../main.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"

#include "y/mkl/scalar.hpp"
#include "y/mkl/api.hpp"
#include "y/ordered/heap.hpp"

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
                        inline SignType operator()(const Unit &lhs, const Unit &rhs) noexcept
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
                    inline explicit Code_() noexcept {}
                    inline virtual ~Code_() noexcept {}
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Code_);
                };


            };
            

            template <typename T>
            struct AddInterface<T,false>
            {

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

}
Y_UDONE()
