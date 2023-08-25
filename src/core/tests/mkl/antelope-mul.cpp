

#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/mul.hpp"
#include "y/text/justify.hpp"
#include "y/sequence/vector.hpp"

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

    Vector<T> vec;
    for(size_t i=1+ran.leq(5);i>0;--i)
    {
        const T tmp = amp*Bring<T>::Get(ran);
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

#if 0
    Y_SHOW_UNIT(apn);
    Y_SHOW_UNIT(apz);
    Y_SHOW_UNIT(apq);
#endif


}
Y_UDONE()
