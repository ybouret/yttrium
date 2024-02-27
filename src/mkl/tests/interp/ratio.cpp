

#include "y/mkl/interpolation/rational.hpp"
#include "y/utest/run.hpp"
#include <cmath>
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;


namespace
{

}


Y_UTEST(interp_ratio)
{

    MKL::RationalInterpolation<float> ratint;

    Vector<float> xa;
    Vector<float> ya;
    float dy = 0;

    xa << 1;
    ya << 1;

    std::cerr << ratint(0,xa,ya,dy) << " : dy=" << dy << std::endl;

    xa << 2;
    ya << 1.2;
    std::cerr << ratint(0,xa,ya,dy) << " : dy=" << dy << std::endl;

    xa << 3;
    ya << 1.1;
    std::cerr << ratint(0,xa,ya,dy) << " : dy=" << dy << std::endl;


    OutputFile fp("ratint.dat");
    for(size_t i=1;i<=xa.size();++i)
    {
        fp("%g %g\n", xa[i], ya[i]);
    }



}
Y_UDONE()
