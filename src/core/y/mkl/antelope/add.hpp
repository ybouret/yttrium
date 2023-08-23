//! \file

#ifndef Y_MKL_Antelope_Add_Included
#define Y_MKL_Antelope_Add_Included 1

#include "y/mkl/antelope/wary.hpp"
#include "y/mkl/scalar.hpp"
#include "y/mkl/api.hpp"
#include "y/comparison.hpp"
#include "y/ordered/heap.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            template <typename T,const bool> struct AddProxy;
        }
    }

}


namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            template <typename T>
            struct AddProxy<T,true>
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
                    //__________________________________________________________
                    //
                    //! Comparator for Heap of Units
                    //__________________________________________________________
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
                    //! Memory Buffer model for Units
                    //__________________________________________________________
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

                    //! retrieve original value by cast operator
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


                //! Code to use units and heap
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


        }

    }

}

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            template <typename T>
            struct AddProxy<T,false>
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

        }
    }

}

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {

            template <typename T>
            class Add : public AddProxy<T,Wary<T>::Flag>::Code
            {
            public:
                Y_ARGS_DECL(T,Type);

                typedef typename AddProxy<T,Wary<T>::Flag>::Code CodeType;

                explicit Add() : CodeType() {}
                virtual ~Add() noexcept {}
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Add);
            };

        }

    }

}


#endif
