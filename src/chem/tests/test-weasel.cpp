

#include "y/chem/weasel.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(weasel)
{
    Chemical::Weasel       &wsl = Chemical::Weasel::Instance();
    Chemical::Library       lib;
    Chemical::LuaEquilibria eqs;


    if(argc>1)
    {
        wsl( Jive::Module::OpenFile(argv[1]), lib, eqs);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

}
Y_UDONE()

