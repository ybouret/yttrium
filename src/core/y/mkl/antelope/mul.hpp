
//! \file

#ifndef Y_MKL_Antelope_Mul_Included
#define Y_MKL_Antelope_Mul_Included 1

#include "y/mkl/antelope/wary.hpp"
#include "y/mkl/api.hpp"
#include "y/data/small/heavy/list/solo.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
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
            template <typename> class MulUnit;

            template <typename T>
            class MulUnit< XReal<T> >
            {
            public:
                typedef XReal<T> Type;
                const Type       value;

                inline  MulUnit(const T args) noexcept : value(args)                {}
                inline  MulUnit(const MulUnit &other) noexcept : value(other.value) {}
                inline ~MulUnit() noexcept                                          {}

                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.value.exponent,rhs.value.exponent);
                }

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };


            template <typename T>
            class MulUnit< Complex<T> >
            {
            public:
                typedef Complex<T> Type;
                const Type value;
                const int  exponent;

                inline MulUnit(const Type args) noexcept : value(args), exponent(0)
                {
                    const T av = Fabs<Type>::Of(value);
                    (void) std::frexp(av, & Coerce(exponent) );
                }

                inline  MulUnit(const MulUnit &other) noexcept : value(other.value), exponent(other.exponent) {}
                inline ~MulUnit() noexcept {}

                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.exponent,rhs.exponent);
                }

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            template <typename T>
            class MulUnit< Complex< XReal<T> > >
            {
            public:
                typedef Complex< XReal<T> > Type;
                const Type value;
                const int  exponent;

                inline MulUnit(const Type args) noexcept : value(args), exponent(0)
                {
                    const XReal<T> av = Fabs<Type>::Of(value);
                    Coerce(exponent) = av.exponent;
                }

                inline  MulUnit(const MulUnit &other) noexcept : value(other.value), exponent(other.exponent) {}
                inline ~MulUnit() noexcept {}

                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.exponent,rhs.exponent);
                }

            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            template <typename T>
            class MulUnit
            {
            public:
                typedef T Type;
                const Type value;
                const int  exponent;

                inline MulUnit(const Type args) noexcept :
                value(args), exponent(0)
                {
                    (void) std::frexp(value, & Coerce(exponent) );
                }

                inline  MulUnit(const MulUnit &other) noexcept : value(other.value), exponent(other.exponent) {}
                inline ~MulUnit() noexcept {}

                static inline SignType Compare(const MulUnit &lhs, const MulUnit &rhs) noexcept
                {
                    return Sign::Of(lhs.exponent,rhs.exponent);
                }


            private:
                Y_DISABLE_ASSIGN(MulUnit);
            };

            template <typename T>
            inline std::ostream & operator<<(std::ostream &os, const MulUnit<T> &u)
            {
                os << u.value;
                return os;
            }


            template <typename T>
            inline   MulUnit<T> operator * (const MulUnit<T> &lhs, const MulUnit<T> &rhs) noexcept
            {
                return MulUnit<T>(lhs.value*rhs.value);
            }


            template <typename T>
            class MulList
            {
            public:
                typedef MulUnit<T>                     UnitType;
                typedef Small::SoloHeavyList<UnitType> ListType;
                typedef typename ListType::NodeType    NodeType;
                typedef ListOf<NodeType>               CoreList;

                explicit MulList() noexcept : my() {}
                virtual ~MulList() noexcept {}

                inline void make(const size_t n)
                {
                    my.release();
                    const size_t m = my.proxy->stowage();
                    if(m<n)
                    {
                        my.proxy->reserve(n-m);
                        assert(n==my.proxy->stowage());
                    }
                }

                void insert(const T args)
                {
                    const UnitType u(args);
                    pushUnit(u);
                }

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
                        return my.pullHead().value;
                    }
                }

                inline friend std::ostream & operator<<(std::ostream &os, const MulList &self)
                {
                    return os << self.my;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(MulList);
                ListType my;


                inline void pushUnit(const UnitType &u)
                {
                    CoreList lhs; lhs.pushTail( my.proxy->produce(u) );
                    CoreList rhs; rhs.swapWith( my );

                    while(lhs.size>0 && rhs.size>0)
                    {
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

            template <typename T>
            class MulProxy<T,true> : public MulList<T>
            {
            public:
                inline virtual ~MulProxy() noexcept {}

            protected:
                inline explicit MulProxy() noexcept : MulList<T>() {}

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
            template <typename T>
            class MulProxy<T,false>
            {
            public:
                inline virtual ~MulProxy() noexcept {}

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



                inline T product()
                {
                    const T res = state;
                    state = 0;
                    empty = true;
                    return res;
                }


            protected:
                explicit MulProxy() : empty(true), state(0) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MulProxy);
                bool empty;
                T    state;
            };
        }
    }
}



#endif

