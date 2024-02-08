//! \file

#ifndef Y_MKL_Antelope_Add_Included
#define Y_MKL_Antelope_Add_Included 1

#include "y/mkl/api.hpp"
#include "y/mkl/antelope/interface.hpp"
#include "y/mkl/antelope/wary.hpp"
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
            //__________________________________________________________________
            //
            //
            //
            //! Proxy when Heap is required
            //
            //
            //__________________________________________________________________
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
                        inline  Comparator() noexcept {} //!< setup
                        inline ~Comparator() noexcept {} //!< cleanup

                        //! keep smallest absValue on top
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


                //______________________________________________________________
                //
                //
                //
                //! Code to use units and heap
                //
                //
                //______________________________________________________________
                class Code : public Interface, public Heap<Unit,typename Unit::Buffer, typename Unit::Comparator>
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef Heap<Unit,typename Unit::Buffer, typename Unit::Comparator> HeapType; //!< alias
                    static const bool Exact = false; //!< result is not exact
                    using HeapType::size;
                    using HeapType::insert;
                    using HeapType::pull;

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    inline explicit Code()      noexcept : HeapType()             {} //!< setup
                    inline explicit Code(const size_t n) : HeapType(n,AsCapacity) {} //!< setup with capacity
                    inline virtual ~Code()      noexcept                          {} //!< cleanup

                    //! add a new value
                    inline Code & operator<<(const T args)
                    {
                        insert(args);
                        return *this;
                    }

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    inline virtual bool isEmpty() const noexcept {
                        return size() <= 0;
                    }

                    inline virtual bool accepts(const size_t n) const noexcept
                    {
                        return this->capacity() >= n;
                    }

                    inline virtual size_t ability() const noexcept
                    {
                        return this->capacity();
                    }

                    //__________________________________________________________
                    //
                    //
                    //! Sum algorithm
                    //
                    //__________________________________________________________
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

                    //! sum(x1)
                    inline T operator()(const T x1) { return x1; }

                    //! sum(x1,x2)
                    inline T operator()(const T x1, const T x2) { return x1+x2; }

                    //! sum(x1,x2,x3)
                    inline T operator()(const T x1, const T x2, const T x3)
                    {
                        this->free();
                        insert(x1); insert(x2); insert(x3);
                        return sum();
                    }

                    //! sum(x1,x2,x3,x4)
                    inline T operator()(const T x1, const T x2, const T x3, const T x4)
                    {
                        this->free();
                        insert(x1); insert(x2); insert(x3); insert(x4);
                        return sum();
                    }

                    //! sum(x1,x2,x3,x4,x5)
                    inline T operator()(const T x1, const T x2, const T x3, const T x4, const T x5)
                    {
                        this->free();
                        insert(x1); insert(x2); insert(x3); insert(x4); insert(x5);
                        return sum();
                    }

                    //! sum(x1,x2,x3,x4,x5,x6)
                    inline T operator()(const T x1, const T x2, const T x3, const T x4, const T x5, const T x6)
                    {
                        this->free();
                        insert(x1); insert(x2); insert(x3); insert(x4); insert(x5); insert(x6);
                        return sum();
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
            //__________________________________________________________________
            //
            //
            //
            //! Proxy when direct operations
            //
            //
            //__________________________________________________________________
            template <typename T>
            struct AddProxy<T,false>
            {

                //______________________________________________________________
                //
                //
                //
                //! Code to use bare accumulator, mocking Heap behaviour
                //
                //
                //______________________________________________________________
                class Code : public Interface
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    static const bool Exact = true; //!< result is exact

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    explicit Code(const size_t =0) : acc(0), __0(acc) {} //!< initialize, whatever the size
                    virtual ~Code() noexcept                          {} //!< cleanup

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    inline void make(size_t) noexcept { acc.zset(); } //!< initialize, whatever the size
                    inline void free()       noexcept { acc.zset(); } //!< set to zero


                    //! here, zero <=> empty
                    inline virtual bool isEmpty() const noexcept {
                        return __0 == acc;
                    }

                    //! always accepts
                    inline virtual bool accepts(const size_t) const noexcept
                    {
                        return true;
                    }

                    inline virtual size_t ability() const noexcept
                    {
                        return 0;
                    }

                    //! return computed sum, reset
                    inline T    sum()
                    { T res = acc; acc.zset(); return res; }

                    //! add
                    inline Code & operator<<(const T &args)
                    {
                        acc += args;
                        return *this;
                    }

                private:
                    T       acc;
                    const T __0;
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

            //__________________________________________________________________
            //
            //
            //
            //! Addition Protocol depending on type
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Add : public AddProxy<T,Wary<T>::Flag>::Code
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                Y_ARGS_DECL(T,Type);                                       //!< aliases
                typedef typename AddProxy<T,Wary<T>::Flag>::Code CodeType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Add() : CodeType() {}                //!< setup
                explicit Add(const size_t n) : CodeType(n) {} //!< setup with capacity
                virtual ~Add() noexcept {}                    //!< cleanup
                

                //______________________________________________________________
                //
                //
                //
                //
                //______________________________________________________________

                //! compute norm of readable-like array[1..arr.size()]
                template <typename ARRAY>
                inline T normOf(ARRAY &arr)
                {

                    //----------------------------------------------------------
                    // check size
                    //----------------------------------------------------------
                    const T      zero(0);
                    if(arr.size()<=0) return zero;

                    //----------------------------------------------------------
                    // find maximum of absolute values
                    //----------------------------------------------------------
                    size_t  imax = 1;
                    const T amax = findMax(arr,imax);
                    if(amax<=zero) return zero;

                    //----------------------------------------------------------
                    // return norm
                    //----------------------------------------------------------
                    return amax * scaledSqrt(arr,amax,imax);
                }

                template <typename ARRAY>
                inline void normalize(ARRAY &arr)
                {
                    const size_t n = arr.size();           if(n<=0) return;
                    size_t       imax = 1;
                    const T      zero(0);
                    const T      amax = findMax(arr,imax); if(amax<=zero) return;
                    const T      scal = amax * scaledSqrt(arr,amax,imax);
                    for(size_t i=n;i>0;--i) arr[i] /= scal;
                }


                //______________________________________________________________
                //
                //
                // Statistical Methods
                //
                //______________________________________________________________

                //! average of a range
                template <typename ITERATOR> inline
                T average(ITERATOR it, const size_t n)
                {
                    switch(n)
                    {
                        case 0: return T(0);
                        case 1: return *it;
                        default: break;
                    }
                    this->make(n);
                    for(size_t i=n;i>0;--i,++it)
                        (*this) << *it;
                    return this->sum()/T(n);
                }

                //! average of a sequence
                template <typename SEQUENCE> inline
                T average(SEQUENCE &seq)
                {
                    return average(seq.begin(),seq.size());
                }

                //! sum(|range-ave|^2)
                template <typename ITERATOR, typename U>
                T sumMod2(ITERATOR it, const size_t n, const U &ave)
                {
                    if(n<=0)
                    {
                        return Mod2<U>::Of(ave);
                    }
                    else
                    {
                        this->make(n);
                        for(size_t i=n;i>0;--i,++it)
                        {
                            const U delta = *it - ave;
                            (*this) << Mod2<U>::Of(delta);
                        }
                        return this->sum();
                    }
                }




            private:
                Y_DISABLE_COPY_AND_ASSIGN(Add);

                template <typename ARRAY>
                inline T findMax(ARRAY &arr, size_t &imax)
                {
                    assert(arr.size()>0);
                    assert(1==imax);
                    T      amax = Fabs<T>::Of(arr[1]);
                    for(size_t i=arr.size();i>1;--i)
                    {
                        const T atmp = Fabs<T>::Of(arr[i]);
                        if(atmp>amax)
                        {
                            amax = atmp;
                            imax = i;
                        }
                    }
                    return amax;
                }

                //! push (|arr|/amax)^2
                template <typename ARRAY>
                inline T scaledSqrt(ARRAY &arr, const T &amax, size_t imax)
                {
                    const size_t n    = arr.size();
                    Add<T>      &self = *this;
                    self.make(n); assert(self.isEmpty());
                    {
                        const T one(1);
                        self << one;
                    }

                    for(size_t i=n;i>imax;--i)
                    {
                        const T tmp = Fabs<T>::Of(arr[i])/amax;
                        self << tmp*tmp;
                    }

                    for(size_t i=--imax;i>0;--i)
                    {
                        const T tmp = Fabs<T>::Of(arr[i])/amax;
                        self << tmp*tmp;
                    }

                    const T s2 = self.sum();
                    return Sqrt<T>::Of(s2);
                }


            };

        }

    }

}


#endif
