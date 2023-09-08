

#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/mul.hpp"
#include "y/text/justify.hpp"
#include "y/sequence/vector.hpp"
#include <cstring>

using namespace Yttrium;
using namespace MKL;


namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            
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



Y_UTEST(mkl_xmul)
{
    Random::Rand ran;

    typedef Antelope::MulUnit<long double> Unit;
    typedef Small::HeavyNode<Unit>         Node;
    Y_SIZEOF(Unit);
    Y_SIZEOF(Node);
    void *obj = Object:: operator new(64);
    const Unit u = 1000;
    void *wksp[10];
    new (wksp) Node(u);
    new (obj)  Node(u);

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
