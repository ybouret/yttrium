#include "y/mkl/antelope/sum3.hpp"

#include "../main.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace MKL;


Y_UTEST(mkl_sum3)
{

    {
        double a=1, b=-2, c=3;
        std::cerr << Antelope::Sum3<double>::Of(a,b,c) << std::endl;
    }

    {
        Complex<long double> a(1), b(-2), c(3);
        std::cerr << Antelope::Sum3< Complex<long double> >::Of(a,b,c) << std::endl;
    }

    {
        apz a(1), b(-2), c(3);
        std::cerr << Antelope::Sum3< apz >::Of(a,b,c) << std::endl;

    }

}
Y_UDONE()

