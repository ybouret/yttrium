
#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Library lib;
    std::cerr << lib << std::endl;

    const Species &h = lib("H^+",1);
    const Species &w = lib("HO^-",-1);

    std::cerr << h   << std::endl;
    std::cerr << w   << std::endl;
    std::cerr << lib << std::endl;

    
    Weasel &weasel = Weasel::Instance();
    std::cerr << weasel.callSign() << std::endl;
    if(argc>1)
    {
        weasel( Lingo::Module::OpenFile(argv[1]) );
    }
}
Y_UDONE()
