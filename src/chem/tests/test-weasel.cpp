
#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(weasel)
{
    
    Weasel   &weasel = Weasel::Instance();
    Library   lib;
    std::cerr << weasel.callSign() << " is ready" << std::endl;
    if(argc>1)
    {
       weasel(lib,Lingo::Module::OpenFile(argv[1]) );
    }
    
    std::cerr << "lib=" << lib << std::endl;


}
Y_UDONE()
