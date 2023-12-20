#include "y/mkl/antelope/sum3.hpp"

#include "../main.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace MKL;


Y_UTEST(mkl_sum3)
{

    
    {
        std::cerr << "double" << std::endl;
        double a=1, b=-2, c=3;
        std::cerr << Antelope::Sum3<double>::Of(a,b,c)    << std::endl;
        std::cerr << Antelope::Sum3<double>::OfAbs(a,b,c) << std::endl;
    }

    {
        std::cerr << "Complex" << std::endl;
        Complex<long double> a(1), b(-2), c(3);
        std::cerr << Antelope::Sum3< Complex<long double> >::Of(a,b,c) << std::endl;
        std::cerr << Antelope::Sum3< Complex<long double> >::OfAbs(a,b,c) << std::endl;

    }

    {
        std::cerr << "apz" << std::endl;
        apz a(1), b(-2), c(3);
        std::cerr << Antelope::Sum3< apz >::Of(a,b,c)    << std::endl;
        std::cerr << Antelope::Sum3< apz >::OfAbs(a,b,c) << std::endl;


    }

}
Y_UDONE()

