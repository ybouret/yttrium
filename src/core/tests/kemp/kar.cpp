#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/kemp/element.hpp"

using namespace Yttrium;

static inline
std::ostream & show( std::ostream &os, const uint64_t u )
{
    std::cerr << Hexadecimal(u) << " / " << std::setw(3) << BitCount::For(u) << "  (" << u << ")";
    return os;
}

static inline
void kar(const uint64_t x, const uint64_t y)
{
    const uint64_t x1 = (x>>16);
    const uint64_t x0 = (x&0xffff);

    const uint64_t y1 = (y>>16);
    const uint64_t y0 = (y&0xffff);

    show(std::cerr << "x  = ",  x) << std::endl;
    show(std::cerr << "x0 = ", x0) << std::endl;
    show(std::cerr << "x1 = ", x1) << std::endl;
    show(std::cerr << "y  = ",  y) << std::endl;
    show(std::cerr << "y0 = ", y0) << std::endl;
    show(std::cerr << "y1 = ", y1) << std::endl;


    const uint64_t z0 = x0*y0;
    const uint64_t z2 = x1*y1;
    const uint64_t z3 = (x0+x1)*(y0+y1); // 17*2 = 34 bits max
    const uint64_t z1 = (z3-z2-z0);
    show(std::cerr << "z0 = ", z0) << std::endl;
    show(std::cerr << "z2 = ", z2) << std::endl;
    show(std::cerr << "z3 = ", z3) << std::endl;
    show(std::cerr << "z1 = ", z1) << std::endl;
    Y_ASSERT(x0*y1+y0*x1==z1);
    const uint64_t p0 = z0;
    const uint64_t p1 = (z1<<16);
    const uint64_t p2 = (z2<<32);
    show(std::cerr << "p0 = ", p0) << std::endl;
    show(std::cerr << "p1 = ", p1) << std::endl;
    show(std::cerr << "p2 = ", p2) << std::endl;
    const uint64_t p = z0 +  p1 + p2;
    const uint64_t xy = x*y;
    show(std::cerr << "p  = ",  p) << std::endl;
    show(std::cerr << "xy = ",  xy) << std::endl;
    Y_CHECK(xy==p);
    std::cerr << std::endl;




}

Y_UTEST(kemp_kar)
{
    Random::ParkMiller ran;

    {
        const uint64_t x  = ran.to<uint32_t>();
        const uint64_t y  = ran.to<uint32_t>();
        kar(x,y);
    }
    kar(0xffffffff,0xffffffff);


}
Y_UDONE()

