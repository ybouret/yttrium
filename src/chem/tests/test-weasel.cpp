

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
#if 0
        //Jive::Syntax::XTree ast = chemlang( Jive::Module::Open( argv[1]) );
        //if(ast.isValid())
        {
            Vizible::GraphViz( "chem.dot", *ast);
            Chemical::Linker linker;
            linker(*ast,lib,eqs);
        }
#endif
    }

    std::cerr << "lib=" << lib << std::endl;

}
Y_UDONE()

