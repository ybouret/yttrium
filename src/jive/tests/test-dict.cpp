#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/pattern/posix.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(dict)
{
    Jive::Dictionary dict;

    std::cerr << dict << std::endl;
    dict("lower", Jive::posix::lower() );
    std::cerr << dict << std::endl;
    dict("upper", Jive::posix::upper() );
    std::cerr << dict << std::endl;

}
Y_UDONE()

