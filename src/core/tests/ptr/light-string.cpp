

#include "y/ptr/light-string.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(ptr_light_string)
{
    LightString p = "Hello";

    std::cerr << p << std::endl;

    const LightString q("Hello"," World");

    std::cerr << q << " / " << q() << std::endl;

}
Y_UDONE()
