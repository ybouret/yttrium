
#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Weasel &weasel = Weasel::Instance();
    std::cerr << weasel.callSign() << std::endl;
}
Y_UDONE()
