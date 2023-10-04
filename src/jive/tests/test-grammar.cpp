
#include "y/jive/syntax/xnode.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Jive;

Y_UTEST(grammar)
{
    Y_SIZEOF(Syntax::XNode);
    Y_SIZEOF(Lexical::Unit);
    Y_SIZEOF(Syntax::XNode::List_);
    Y_SIZEOF(Syntax::XNode::List);

}
Y_UDONE()


