

#include "y/jive/syntax/terminal.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            Terminal:: ~Terminal() noexcept
            {
            }

            bool Terminal:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                Lexeme *lexeme = lexer.get(source);
                if(!lexeme)
                {
                    return false; // End Of Stream
                }
                else
                {
                    if( *(lexeme->name) == *name )
                    {
                        XNode::Expand(tree, XNode::Create(*this,lexeme) );
                        return true;
                    }
                    else
                        return false;
                }
            }

        }

    }

}


