
#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/add.hpp"
#include "y/text/justify.hpp"

using namespace Yttrium;
using namespace MKL;




template <typename T>
static inline void ShowUnit( const char *name, Random::Bits &ran )
{
    std::cerr << '[' << Justify(name,32,Justify::Center) << ']' << std::endl;
    std::cerr << std::setw(16) << sizeof( typename MKL::Antelope::AddProxy<T,true>::Unit ) << std::endl;

    MKL::Antelope::Add<T> xadd;

    xadd.free();

    std::cerr << xadd.sum();

    for(size_t i=1+ran.leq(10);i>0;--i)
    {
        const T tmp = Bring<T>::Get(ran);
        std::cerr << "+(" << tmp << ")";
        xadd.insert( tmp );
    }
    std::cerr << " = " << xadd.sum() << std::endl;

}

#define Y_SHOW_UNIT(CLASS) ShowUnit<CLASS>( #CLASS, ran)



Y_UTEST(mkl_antelope)
{
    Random::Rand ran;

    Y_SHOW_UNIT(float);
    Y_SHOW_UNIT(double);
    Y_SHOW_UNIT(long double);

    Y_SHOW_UNIT(Complex<float>);
    Y_SHOW_UNIT(Complex<double>);
    Y_SHOW_UNIT(Complex<long double>);


    Y_SHOW_UNIT(XReal<float>);
    Y_SHOW_UNIT(XReal<double>);
    Y_SHOW_UNIT(XReal<long double>);
    Y_SHOW_UNIT(Complex< XReal<float> >);
    Y_SHOW_UNIT(Complex< XReal<double> >);
    Y_SHOW_UNIT(Complex< XReal<long double> >);




}
Y_UDONE()
