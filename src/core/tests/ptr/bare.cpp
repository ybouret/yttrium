
#include "y/ptr/bare.hpp"
#include "y/ptr/drill-down.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(ptr_bare)
{

    String          s  = "Hello, World!";
    BarePtr<String> p1 = 0;

    std::cerr << p1 << std::endl;
    p1 = &s;
    std::cerr << p1 << std::endl;
    p1 = 0;
    std::cerr << p1 << std::endl;

}
Y_UDONE()

