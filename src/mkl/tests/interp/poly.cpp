
#include "y/mkl/interpolation/polynomial.hpp"
#include "y/utest/run.hpp"
#include <cmath>
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/interpolation/linear.hpp"

using namespace Yttrium;


namespace
{

}


Y_UTEST(interp_poly)
{

    MKL::PolynomialInterpolation<float> polint;

    Vector<float> xa;
    Vector<float> ya;
    float dy = 0;

    xa << 1;
    ya << 1;

    std::cerr << polint(0,xa,ya,dy) << " : dy=" << dy << std::endl;

    xa << 2;
    ya << 1.2;
    std::cerr << polint(0,xa,ya,dy) << " : dy=" << dy << std::endl;

    xa << 3;
    ya << 1.1;
    std::cerr << polint(0,xa,ya,dy) << " : dy=" << dy << std::endl;


    {
        OutputFile fp("polint.dat");
        for(size_t i=1;i<=xa.size();++i)
        {
            fp("%g %g\n", xa[i], ya[i]);
        }
    }

    MKL::LinearInterpolation<float> linear;
    {
        OutputFile fp("linear.dat");
        for(size_t i=0;i<=100;++i)
        {
            const float x = xa.head() + float(i) * (xa.tail() - xa.head()) / 100;
            fp("%g %g\n", x, linear(x,xa,ya));
        }
    }



}
Y_UDONE()
