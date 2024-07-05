
#include "y/chemical/weasel/parser.hpp"
#include "y/utest/run.hpp"
#include "y/jive/syntax/translator.hpp"

//#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Weasel::Parser parse("Weasel");
    if(argc>1)
    {
        AutoPtr<Weasel::XNode> ast = parse(Jive::Module::OpenFile(argv[1]));
        GraphViz::Vizible::DotToPng( "ast.dot", *ast);
        Jive::Syntax::Translator  analyzer;
        analyzer.translate(*ast,Yttrium::Jive::Syntax::Permissive);
    }



}
Y_UDONE()
