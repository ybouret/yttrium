
#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/weasel/linker.hpp"
#include "y/utest/run.hpp"

//#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Weasel::Parser parser("Weasel");
    Weasel::Linker link;
    Library        lib;
    LuaEquilibria  eqs;

    if(argc>1)
    {
        AutoPtr<Weasel::XNode> ast = parser.load(Jive::Module::OpenFile(argv[1]));
        GraphViz::Vizible::DotToPng( "ast.dot", *ast);
        link(*ast,lib,eqs);
    }



}
Y_UDONE()
