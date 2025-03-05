#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/formula.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Weasel::Parser wp;

    if( argc > 1)
    {
        AutoPtr<XNode> xnode = wp( Lingo::Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng("xnode.dot", *xnode);
    }

    Y_SIZEOF(XNode);
}
Y_UDONE()

