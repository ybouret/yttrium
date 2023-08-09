#include "y/jive/pattern/first-chars.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

Y_UTEST(fc)
{
    FirstChars fc;

    std::cerr << fc << std::endl;
    fc.include('a');     std::cerr << fc << std::endl;
    fc.include('a','z'); std::cerr << fc << std::endl;
    fc.include('0','9'); std::cerr << fc << std::endl;
    fc.exclude('6');     std::cerr << fc << std::endl;
    fc.exclude('j','q'); std::cerr << fc << std::endl;



}
Y_UDONE()

