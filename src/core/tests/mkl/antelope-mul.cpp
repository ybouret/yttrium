

#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/add.hpp"
#include "y/text/justify.hpp"
#include "y/sequence/vector.hpp"
#include "y/data/small/heavy/list/solo.hpp"

using namespace Yttrium;
using namespace MKL;


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
            class MulList : public Small::SoloHeavyList< MulUnit<T> >
            {
            public:
                typedef MulUnit<T> UnitType;
                typedef Small::SoloHeavyList<UnitType> ListType;
                typedef typename ListType::NodeType    NodeType;
                typedef ListOf<NodeType>               CoreList;

                explicit MulList() noexcept : ListType() {}
                virtual ~MulList() noexcept {}


                void push(const T args)
                {
                    CoreList lhs;
                    {
                        const UnitType u(args);
                        lhs.pushTail( this->proxy->produce(u) );
                    }
                    CoreList rhs; rhs.swapWith(*this);

                    while(lhs.size>0 && rhs.size>0)
                    {
                        switch( UnitType::Compare( **lhs.head, **rhs.head) )
                        {
                            case Negative:
                            case __Zero__:
                                this->pushTail(lhs.popHead());
                                break;
                            case Positive:
                                this->pushTail(rhs.popHead());
                                break;
                        }
                    }
                    assert(0==lhs.size||0==rhs.size);
                    this->mergeTail(lhs);
                    this->mergeTail(rhs);
                }

                void push(const T args, size_t n)
                {
                    CoreList lhs;
                    {
                        const UnitType u(args);
                        while(n-- > 0)
                        {
                            lhs.pushTail( this->proxy->produce(u) );
                        }
                    }
                    CoreList rhs; rhs.swapWith(*this);

                    while(lhs.size>0 && rhs.size>0)
                    {
                        switch( UnitType::Compare( **lhs.head, **rhs.head) )
                        {
                            case Negative:
                            case __Zero__:
                                this->mergeTail(lhs);
                                break;

                            case Positive:
                                this->pushTail(rhs.popHead());
                                break;
                        }
                    }
                    assert(0==lhs.size||0==rhs.size);
                    this->mergeTail(lhs);
                    this->mergeTail(rhs);
                }





            private:
                Y_DISABLE_COPY_AND_ASSIGN(MulList);
            };

        }
    }
}

template <typename T>
static inline void ShowUnit( const char *name, Random::Bits &ran )
{
    std::cerr << '[' << Justify(name,32,Justify::Center) << ']' << std::endl;

    Y_SIZEOF( MKL::Antelope::MulUnit<T> );

    const T amp(100000);

    MKL::Antelope::MulUnit<T> a(amp*Bring<T>::Get(ran));
    MKL::Antelope::MulUnit<T> b(amp*Bring<T>::Get(ran));
    std::cerr << "a=" << a << std::endl;
    std::cerr << "b=" << b << std::endl;
    MKL::Antelope::MulUnit<T> c = a*b;
    std::cerr << "c=" << c << std::endl;
    std::cerr << "compare(a,b)=" << MKL::Antelope::MulUnit<T>::Compare(a,b) << std::endl;


    MKL::Antelope::MulList<T> xmul;


    xmul.push( amp*Bring<T>::Get(ran));
    xmul.push( amp*Bring<T>::Get(ran));
    xmul.push( amp*Bring<T>::Get(ran) );
    xmul.push( amp*Bring<T>::Get(ran), 4);

    std::cerr << xmul << std::endl;


}

#define Y_SHOW_UNIT(CLASS) ShowUnit<CLASS>( #CLASS, ran )



Y_UTEST(mkl_xmul)
{
    Random::Rand ran;

    Y_SHOW_UNIT(float);
    return 0;
    Y_SHOW_UNIT(double);
    Y_SHOW_UNIT(long double);

    Y_SHOW_UNIT(XReal<float>);
    Y_SHOW_UNIT(XReal<double>);
    Y_SHOW_UNIT(XReal<long double>);

    Y_SHOW_UNIT(Complex<float>);
    Y_SHOW_UNIT(Complex<double>);
    Y_SHOW_UNIT(Complex<long double>);




    Y_SHOW_UNIT(Complex< XReal<float> >);
    Y_SHOW_UNIT(Complex< XReal<double> >);
    Y_SHOW_UNIT(Complex< XReal<long double> >);

#if 0
    Y_SHOW_UNIT(apn);
    Y_SHOW_UNIT(apz);
    Y_SHOW_UNIT(apq);
#endif


}
Y_UDONE()
