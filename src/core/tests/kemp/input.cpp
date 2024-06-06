#include "y/kemp/rational.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(kemp_input)
{
    String s = "1234";
    if(argc>1)
    {
        s = argv[1];
    }
    apn n = s;
    std::cerr << s << " => " << n.toDec() << " / 0x" << n.toHex() << std::endl;
}
Y_UDONE()

