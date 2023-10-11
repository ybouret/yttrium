
#include "y/chem/lang/parser.hpp"
#include "y/jive/syntax/analyzer.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(lang)
{
    Chemical::Parser chemlang;

    if(argc>1)
    {
        Jive::Syntax::XTree ast = chemlang( Jive::Module::Open( argv[1]) );
        if(ast.isValid())
        {
            Vizible::GraphViz( "chem.dot", *ast);
            Jive::Syntax::Analyzer  analyzer;
            analyzer.run(*ast);
        }
    }

}
Y_UDONE()

