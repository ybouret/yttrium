
#include "y/chem/weasel.hpp"
#include "y/chem/plexus.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(plexus)
{
    Chemical::Weasel       &wsl = Chemical::Weasel::Instance();
    Chemical::Library       lib;
    Chemical::LuaEquilibria eqs;

    for(int i=1;i<argc;++i)
    {
        const String data = argv[i];
        wsl( Jive::Module::OpenData(data,data), lib, eqs );
    }


    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool  verbose = true;
    XMLog xml(verbose);
    Chemical::Plexus plexus(eqs,xml,0.0);

    std::cerr << eqs << std::endl;

    plexus.graphViz();
    

    

}
Y_UDONE()

