
#include "y/lingo/parser.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Parser:: ~Parser() noexcept
        {

        }


        Syntax::XNode * Parser:: operator()(Source &source)
        {
            lexer.restart();
            XNode * const cst = accept(lexer,source);
            return XNode::AST(cst);
        }

    }

}
