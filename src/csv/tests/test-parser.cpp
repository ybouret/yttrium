#include "y/csv/parser.hpp"
#include "y/csv/document.hpp"
#include "y/jive/syntax/xnode.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(parser)
{
    CSV::Parser parser;

    if(argc>1)
    {
        CSV::SharedDocument doc = parser( Jive::Module::OpenFile(argv[1]) );
    }

}
Y_UDONE()
