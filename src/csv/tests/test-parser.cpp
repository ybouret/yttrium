#include "y/csv/parser.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(parser)
{
    CSV::Parser parser;

    if(argc>1)
    {
        AutoPtr<Jive::Syntax::XNode> tree = parser( Jive::Module::OpenFile(argv[1]) );
    }

}
Y_UDONE()
