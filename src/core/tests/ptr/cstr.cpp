

#include "y/ptr/cstr.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(ptr_cstr)
{
    LightString p = "Hello";

    std::cerr << p << std::endl;

    const LightString q("Hello"," World");

    std::cerr << q << " / " << q() << std::endl;

}
Y_UDONE()
