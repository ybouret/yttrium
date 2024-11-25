

#include "y/chemical/weasel.hpp"
#include "y/chemical/formula.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(formula)
{

    Weasel &weasel = Weasel::Instance();
    std::cerr << weasel.callSign() << " is ready" << std::endl;
    if(argc>1)
    {
        const Formula formula( argv[1] );
    }


}
Y_UDONE()
