

#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/mul.hpp"
#include "y/text/justify.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/fill.hpp"
#include "y/memory/object/sentries.hpp"

#include <cstring>
#include <typeinfo>

using namespace Yttrium;
using namespace MKL;



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

    std::cerr << "-- Creating List" << std::endl;
    Vector<T> vec;
    for(size_t i=1+ran.leq(5);i>0;--i)
    {
        const T tmp = amp*Bring<T>::Get(ran);
        std::cerr << "-- Inserting " << tmp << std::endl;
        xmul.insert(tmp);
        vec << tmp;
    }

    {
        const T tmp = amp*Bring<T>::Get(ran);
        xmul.insert(tmp, 4);
        for(size_t i=4;i>0;--i) vec << tmp;
    }


    std::cerr << xmul << std::endl;
    const T prod = xmul.product();
    std::cerr << "prod=" << prod << std::endl;
    T p = vec[1];
    for(size_t i=2;i<=vec.size();++i) p *= vec[i];
    std::cerr << "pvec=" << p << std::endl;

    xmul.make(60);
    
}

#define Y_SHOW_UNIT(CLASS) ShowUnit<CLASS>( #CLASS, ran )


template <typename T>
static inline void singleNode(Random::Bits &ran)
{
    std::cerr << "-- " << typeid(T).name() << std::endl;
    typedef Antelope::MulUnit<T>     Unit;
    typedef Small::HeavyNode<Unit>   Node;
    Y_SIZEOF(T);
    Y_SIZEOF(Unit);
    Y_SIZEOF(Node);

    const size_t bs = sizeof(Node);
    std::cerr << "-- acquire object" << std::endl;
    uint8_t *obj = static_cast<uint8_t*>( Object:: operator new(bs) );
    std::cerr << "-- randomizing " << bs << " bytes" << std::endl;
    Random::Fill::Block(obj,bs,ran,0x01,0xff);

    const Unit u(1000);

    {
        const Memory::ObjectSentries check(obj);
        Node *node = new (obj) Node(u);
        std::cerr << "node=" << *node << std::endl;
        Memory::OutOfReach::Naught(node);
    }


    Object:: operator delete(obj,bs);
    std::cerr << std::endl;
}


namespace
{

    template <typename T>
    class MulKnot
    {
    public:

        inline MulKnot(const T x) noexcept :
        next(0),
        prev(0),
        exponent(0)
        {
            //(void) std::frexp(value, &Coerce(exponent) );
            (void) std::frexp( value() = x, &Coerce(exponent) );
        }

        inline ~MulKnot() noexcept {}

        inline friend std::ostream & operator<<(std::ostream &os, const MulKnot &knot)
        {
            return os << '<' << knot.value() << '>';
        }

        MulKnot     *next;
        MulKnot     *prev;
        const int    exponent;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MulKnot);
        void  *wksp[ Y_WORDS_FOR(T) ];
        inline T &value() const noexcept { return *Memory::OutOfReach::Cast<T>( Coerce(wksp) ); }
    };


    template <typename T>
    static inline void testKnots(Random::Bits &ran)
    {

        std::cerr << "-- " << typeid(T).name() << std::endl;
        Y_SIZEOF(T);
        Y_SIZEOF(MulKnot<T>);
        const size_t bs = sizeof(MulKnot<T>);
        std::cerr << "-- acquire object" << std::endl;
        uint8_t *obj = static_cast<uint8_t*>( Object:: operator new(bs) );
        std::cerr << "-- randomizing " << bs << " bytes" << std::endl;
        {
            const Memory::ObjectSentries check(obj);
             Random::Fill::Block(obj,bs,ran,0x01,0xff);
        }
        const T u = 1000;

        MulKnot<T> *knot = new (obj) MulKnot<T>(u);
        std::cerr << *knot << std::endl;

        Object:: operator delete(obj,bs);

    }

}


Y_UTEST(mkl_xmul)
{
    Random::Rand ran;

    testKnots<float>(ran);
    testKnots<double>(ran);
    testKnots<long double>(ran);

    return 0;

    singleNode<float>(ran);
    singleNode<double>(ran);
    //singleNode<long double>(ran);

    singleNode< XReal<float> >(ran);
    singleNode< XReal<double> >(ran);
    //singleNode< XReal<long double> >(ran);


    singleNode< Complex<float>  >(ran);
    singleNode< Complex<double> >(ran);
    //singleNode< Complex<long double> >(ran);



    //Y_SHOW_UNIT(float);
    //Y_SHOW_UNIT(double);
    //Y_SHOW_UNIT(long double);

    return 0;

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
