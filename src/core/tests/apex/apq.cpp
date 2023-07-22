


#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_q)
{
    { apq q;      std::cerr << q << std::endl; }
    { apq q(5,2); std::cerr << q << std::endl; }
    { apq q(-6,14); std::cerr << q << std::endl; }

    {
        apq a(1,2);
        apq b(3,2);
        std::cerr << a+b << std::endl;
        b = apq(-1,2);
        std::cerr << a+b << std::endl;
        std::cerr << a+2 << std::endl;
        std::cerr << -3+a << std::endl;
        apz z =  -2;
        std::cerr << a+z << std::endl;
        std::cerr << z+a << std::endl;
        apn n = 2;
        std::cerr << a+n << std::endl;
        std::cerr << n+a << std::endl;
    }

}
Y_UDONE()
