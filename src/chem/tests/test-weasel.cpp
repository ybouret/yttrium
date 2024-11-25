
#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(weasel)
{
    
    Weasel &weasel = Weasel::Instance();
    std::cerr << weasel.callSign() << " is ready" << std::endl;
    if(argc>1)
    {
        weasel( Lingo::Module::OpenFile(argv[1]) );
    }
}
Y_UDONE()
