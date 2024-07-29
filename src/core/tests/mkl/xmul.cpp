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
    std::cerr << std::endl;
    std::cerr << '[' << Justify(name,32,Justify::Center) << ']' << std::endl;

    Y_SIZEOF( MKL::Antelope::MulUnit<T> );

    const T amp(1000);

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
static inline void testXMUL(Random::Rand &ran)
{

    Antelope::Mul<T> xmul;
    Vector<T>        smul;

    for(size_t n=1;n<=10;++n)
    {
        xmul.free();
        smul.free();
        for(size_t i=n;i>0;--i)
        {
            const T tmp = Bring<T>::Get(ran);
            xmul << tmp;
            smul << tmp;
        }

        const T xres = xmul.product();
        T       sres(1);
        for(size_t i=smul.size();i>0;--i) sres *= smul[i];
        if( !Antelope::Wary<T>::Flag )
        {
            sres -= xres;
            const typename ScalarFor<T>::Type zero(0);
            Y_CHECK( Fabs<T>::Of(sres) <= zero);
        }
    }

}




Y_UTEST(mkl_xmul)
{
    Random::Rand ran;


    Y_SHOW_UNIT(float);
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


    testXMUL<float>(ran);
    testXMUL<double>(ran);
    testXMUL<long double>(ran);

    testXMUL< XReal<float>       >(ran);
    testXMUL< XReal<double>      >(ran);
    testXMUL< XReal<long double> >(ran);

    testXMUL< Complex<float>       >(ran);
    testXMUL< Complex<double>      >(ran);
    testXMUL< Complex<long double> >(ran);


    testXMUL<  Complex<XReal<float>       > >(ran);
    testXMUL<  Complex<XReal<double>      > >(ran);
    testXMUL<  Complex<XReal<long double> > >(ran);

    testXMUL<apq>(ran);
    testXMUL<apz>(ran);
    testXMUL<apn>(ran);


    {
        const XReal<double> x(1,Raised,-1000); std::cerr << "x=" << x << std::endl;
        const XReal<double> y(7,Raised,-2000); std::cerr << "y=" << y << std::endl;
        const XReal<double> z = x*y;           std::cerr << "z=" << z << std::endl;
        Antelope::Mul< XReal<double> > xmul;
        xmul << x;
        xmul << y;
        std::cerr << xmul << std::endl;
        std::cerr << xmul.product() << std::endl;
    }
}
Y_UDONE()
