
#include "y/chem/lang/parser.hpp"
#include "y/chem/lang/linker.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(lang)
{
    Chemical::Parser        chemlang;
    Chemical::Library       lib;
    Chemical::LuaEquilibria eqs;

    Jive::Syntax::Rule::Verbose = true;

    chemlang.renderGraphViz();

    if(argc>1)
    {
        Jive::Syntax::XTree ast = chemlang( Jive::Module::Open( argv[1]) );
        if(ast.isValid())
        {
            GraphViz::Vizible::DotToPng( "chem.dot", *ast);
            Chemical::Linker linker;
            linker(*ast,lib,eqs);
        }
    }

    std::cerr << "lib=" << lib << std::endl;
    


}
Y_UDONE()

