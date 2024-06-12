#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

Y_UTEST(kemp_kar)
{
    Random::ParkMiller ran;

    {
        const uint64_t x  = ran.to<uint32_t>();
        const uint64_t x1 = (x>>16);
        const uint64_t x0 = (x&0xffff);

        const uint64_t y  = ran.to<uint32_t>();
        const uint64_t y1 = (y>>16);
        const uint64_t y0 = (y&0xffff);

        std::cerr << "x  = "  << Hexadecimal(x)  << std::endl;
        std::cerr << "x0 = " << Hexadecimal(x0) << std::endl;
        std::cerr << "x1 = " << Hexadecimal(x1) << std::endl;

        std::cerr << "y  = "  << Hexadecimal(y)  << std::endl;
        std::cerr << "y0 = " << Hexadecimal(y0) << std::endl;
        std::cerr << "y1 = " << Hexadecimal(y1) << std::endl;

        const uint64_t z0 = x0*y0;
        const uint64_t z2 = x1*y1;
        const uint64_t z3 = (x0+x1)*(y0+y1);
        std::cerr << "z0 = " << Hexadecimal(z0) << std::endl;
        std::cerr << "z2 = " << Hexadecimal(z2) << std::endl;
        std::cerr << "z3 = " << Hexadecimal(z3) << std::endl;
        const uint64_t z1 = (z3-z2-z0);
        std::cerr << "z1 = " << Hexadecimal(z1) << std::endl;
        Y_ASSERT(x0*y1+y0*x1==z1);
        const uint64_t p = z0 +  (z1<<16) + (z2<<32);
        const uint64_t xy = x*y;
        std::cerr << "p  = " << Hexadecimal(p) << std::endl;
        std::cerr << "xy = " << Hexadecimal(xy) << std::endl;
        Y_ASSERT(xy==p);
    }

}
Y_UDONE()

