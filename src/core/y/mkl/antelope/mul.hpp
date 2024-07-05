
//! \file

#ifndef Y_MKL_Antelope_Mul_Included
#define Y_MKL_Antelope_Mul_Included 1

#include "y/mkl/antelope/wary.hpp"
#include "y/mkl/api.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/type/moniker.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            //__________________________________________________________________
            //
            //
            //! proxy to perform multiplication
            //
            //__________________________________________________________________
            template <typename T,const bool> class MulProxy;
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
            //! Prototype to store values
            //
            //__________________________________________________________________
            template <typename> class MulUnit;

            //__________________________________________________________________
            //
            //
            //
            //! Multiplication of extended reals
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulUnit< XReal<T> >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef XReal<T> Type; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline  MulUnit(const T args) noexcept : value(args)                {} //!< setup
                inline  MulUnit(const MulUnit &other) noexcept : value(other.value) {} //!< copy
                inline ~MulUnit() noexcept                                          {} //!< cleanup

                //______________________________________________________________
                //
                //
                //! Comparison
                //
                //______________________________________________________________
                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.value->exponent,rhs.value->exponent);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Moniker<Type> value; //!< internal exponent is pre-computed

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Multiplication of complexes
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulUnit< Complex<T> >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Complex<T> Type; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline MulUnit(const Type args) noexcept : value(args), exponent(0)
                {
                    const T av = Fabs<Type>::Of(*value);
                    (void) std::frexp(av, & Coerce(exponent) );
                }

                //! copy
                inline  MulUnit(const MulUnit &other) noexcept : value(other.value), exponent(other.exponent) {}

                //! cleanup
                inline ~MulUnit() noexcept {}

                //______________________________________________________________
                //
                //
                //! Comparison
                //
                //______________________________________________________________
                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.exponent,rhs.exponent);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Moniker<Type> value;      //!< complex
                const int           exponent;   //!< exponent of its modulus

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Multiplication of extended complexes
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulUnit< Complex< XReal<T> > >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Complex< XReal<T> > Type; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline MulUnit(const Type args) noexcept : value(args), exponent(0)
                {
                    const XReal<T> av = Fabs<Type>::Of(*value);
                    Coerce(exponent) = av.exponent;
                }

                //! copy
                inline  MulUnit(const MulUnit &other) noexcept : value(other.value), exponent(other.exponent) {}

                //! cleanup
                inline ~MulUnit() noexcept {}

                //______________________________________________________________
                //
                //
                //! comparison
                //
                //______________________________________________________________
                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.exponent,rhs.exponent);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Moniker<Type> value;     //!< value
                const int           exponent;  //!< exponent of its modulus

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Multiplication of float|double|long double
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulUnit
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef T Type; //!< alias


                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline MulUnit(const Type args) noexcept : value(args), exponent(0)
                {
                    (void) std::frexp(*value, & Coerce(exponent) );
                }

                //! copy
                inline  MulUnit(const MulUnit &other) noexcept :
                value(other.value),
                exponent(other.exponent)
                {
                }

                //! cleanup
                inline ~MulUnit() noexcept {}

                //______________________________________________________________
                //
                //
                //! Comparison
                //
                //______________________________________________________________
                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.exponent,rhs.exponent);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Moniker<Type> value;     //!< real value
                const int           exponent;  //!< its exponent

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            //__________________________________________________________________
            //
            //
            //! display forwarding
            //
            //__________________________________________________________________
            template <typename T>
            inline std::ostream & operator<<(std::ostream &os, const MulUnit<T> &u)
            {
                os << u.value;
                return os;
            }

            //__________________________________________________________________
            //
            //
            //! universal binary multiplication
            //
            //__________________________________________________________________
            template <typename T>
            inline   MulUnit<T> operator * (const MulUnit<T> &lhs, const MulUnit<T> &rhs) noexcept
            {
                return MulUnit<T>( (*lhs.value)* (*rhs.value) );
            }

            //__________________________________________________________________
            //
            //
            //
            //! Ordered List of MulUnit to perform algorithm
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulList
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef MulUnit<T>                     UnitType; //!< alias
                typedef Small::SoloHeavyList<UnitType> ListType; //!< alias
                typedef typename ListType::NodeType    NodeType; //!< alias
                typedef ListOf<NodeType>               CoreList; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit MulList() noexcept : my() {}  //!< setup empty
                virtual ~MulList() noexcept        {}  //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! free
                inline void free() noexcept { my.free(); }

                //! free and reserve memory
                inline void make(const size_t n)
                {
                    my.free();
                    const size_t m = my.proxy->stowage();
                    if(m<n)
                    {
                        my.proxy->reserve(n-m);
                        assert(n==my.proxy->stowage());
                    }
                }

                //! insert args
                void insert(const T args)
                { const UnitType u(args); pushUnit(u); }

                //! helper
                MulList & operator<<(const T &args) {
                    insert(args);
                    return *this;
                }


                //! insert args^n, one if n==0
                void insert(const T args, size_t n)
                {
                    if(n<=0)
                    {
                        if(my.size<=0)
                        {
                            const T one(1);
                            insert(one);
                        }
                    }
                    else
                    {
                        CoreList lhs;
                        {
                            const UnitType u(args);
                            while(n-- > 0)
                            {
                                lhs.pushTail( my.proxy->produce(u) );
                            }
                        }
                        CoreList rhs; rhs.swapWith(my);

                        while(lhs.size>0 && rhs.size>0)
                        {
                            switch( UnitType::Compare( **lhs.head, **rhs.head) )
                            {
                                case Negative:
                                case __Zero__:
                                    my.mergeTail(lhs);
                                    break;

                                case Positive:
                                    my.pushTail(rhs.popHead());
                                    break;
                            }
                        }
                        assert(0==lhs.size||0==rhs.size);
                        my.mergeTail(lhs);
                        my.mergeTail(rhs);
                    }
                }

                //! product algorithm
                inline T product()
                {
                    if(my.size<=0)
                    {
                        return T(0);
                    }
                    else
                    {
                        while(my.size>1)
                        {
                            const UnitType lhs = my.pullHead(); assert(my.size>0);
                            const UnitType rhs = my.pullTail();
                            const UnitType tmp = lhs * rhs;
                            pushUnit(tmp);
                        }
                        assert(1==my.size);
                        return *(my.pullHead().value);
                    }
                }


                //! display content
                inline friend std::ostream & operator<<(std::ostream &os, const MulList &self)
                {
                    return os << self.my;
                }

            protected:
                ListType my;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MulList);


                inline void pushUnit(const UnitType &u)
                {
                    CoreList lhs; lhs.pushTail( my.proxy->produce(u) );
                    CoreList rhs; rhs.swapWith( my );

                    while(lhs.size>0 && rhs.size>0)
                    {
                        assert(0!=rhs.head);
                        assert(0!=lhs.head);
                        switch( UnitType::Compare( **lhs.head, **rhs.head) )
                        {
                            case Negative:
                            case __Zero__:
                               my.pushTail(lhs.popHead());
                                break;
                            case Positive:
                                my.pushTail(rhs.popHead());
                                break;
                        }
                    }
                    assert(0==lhs.size||0==rhs.size);
                    my.mergeTail(lhs);
                    my.mergeTail(rhs);
                }
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
            //! Multiplication with MulList usage
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulProxy<T,true> : public MulList<T>
            {
            public:
                inline virtual ~MulProxy() noexcept {} //!< cleanup

                //! check if is empty
                inline  bool isEmpty() const noexcept
                {
                    return this->my.size <= 0;
                }
                
            protected:
                //! setup empty
                inline explicit MulProxy() noexcept : MulList<T>() {}

                //! setup with capacity
                inline explicit MulProxy(const size_t n) : MulList<T>()
                {
                    this->make(n);
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(MulProxy);
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
            //! Direct multiplication
            //
            //
            //__________________________________________________________________
            template <typename T>
            class MulProxy<T,false>
            {
            public:
                //______________________________________________________________
                //
                //
                // Simplified Methods
                //
                //______________________________________________________________

                //! free
                inline void free() noexcept
                {
                    clearState();
                }

                //! cleaning and read for any number
                inline void make(size_t)
                {
                    clearState();
                }

                //! insert args
                inline void insert(const T &args)
                {
                    if(empty)
                    {
                        state = args;
                        empty = false;
                    }
                    else
                    {
                        state *= args;
                    }
                }

                //! insert args^n, 1 if n == 0
                inline void insert(const T &args, size_t n)
                {
                    if(n>0)
                    {
                        T p = args; while(--n>0) p *= args;
                        insert(p);
                    }
                    else
                    {
                        const T one(1);
                        insert(one);
                    }
                }

                //! helper
                MulProxy & operator<<(const T &args) {
                    insert(args);
                    return *this;
                }


                //! return product and set empty
                inline T product()
                {
                    const T res = state;
                    clearState();
                    return res;
                }

                //! check if is empty
                inline bool isEmpty() const noexcept
                {
                    return empty;
                }

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~MulProxy() noexcept {}

            protected:
                inline explicit MulProxy() : empty(true), state(0) {}             //!< setup
                inline explicit MulProxy(const size_t) : empty(true), state(0) {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MulProxy);
                inline void clearState() { state=0; empty=true; }
                bool empty;
                T    state;
                
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
            //! Multiplication Protocol depending on type
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Mul : public MulProxy<T,Wary<T>::Flag>
            {
            public:
                //__________________________________________________________
                //
                //
                // Definitions
                //
                //__________________________________________________________
                Y_ARGS_DECL(T,Type);                        //!< aliases
                typedef MulProxy<T,Wary<T>::Flag> CodeType; //!< alias

                //__________________________________________________________
                //
                //
                // C++
                //
                //__________________________________________________________
                explicit Mul() : CodeType() {}                //!< setup
                explicit Mul(const size_t n) : CodeType(n) {} //!< setup
                virtual ~Mul() noexcept {}                    //!< cleanup


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Mul);
            };

        }

    }

}



#endif

