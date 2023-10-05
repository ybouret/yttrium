
#include "y/jive/syntax/xnode.hpp"
#include "y/jive/syntax/terminal.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

Y_UTEST(grammar)
{
    Y_SIZEOF(Syntax::XNode);
    Y_SIZEOF(Lexical::Unit);
    Y_SIZEOF(Syntax::XNode::List);

    const Syntax::Terminal term("INT");

    //AutoPtr<Syntax::XNode> xn = Syntax::XNode::Create();

}
Y_UDONE()


