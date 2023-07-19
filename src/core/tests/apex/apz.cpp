

#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_z)
{
    Random::Rand ran;
    {
        apz z;
        std::cerr << z << std::endl;
    }

    {
        apz z = 92;
        std::cerr << z << std::endl;
    }

    {
        apz z = -719;
        std::cerr << z << std::endl;
        z =0;
        std::cerr << z << std::endl;
        z = 8001;
        std::cerr << z << std::endl;
        z = -899;
        std::cerr << z << std::endl;
    }
}
Y_UDONE()
