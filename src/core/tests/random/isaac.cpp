
#include "y/random/isaac.hpp"
#include "y/utest/run.hpp"
#include "y/system/seed.hpp"
#include <cstring>
#include <cstdio>



using namespace Yttrium;

Y_UTEST(random_isaac)
{

    Random::ISAAC<8> ran8(0);
    ran8.outputTest();

    Random::ISAAC<4> ran4(0);

    Random::ISAAC<8> ran;
}
Y_UDONE()

