
#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(weasel)
{
    
    Weasel   &weasel = Weasel::Instance();
    Library   lib;
    Equilibria eqs;

    std::cerr << weasel.callSign() << " is ready" << std::endl;
    if(argc>1)
    {
       weasel(lib,eqs,Lingo::Module::OpenFile(argv[1]) );
    }
    
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    GraphViz::Vizible::DotToPng("eqs.dot", eqs);


}
Y_UDONE()
