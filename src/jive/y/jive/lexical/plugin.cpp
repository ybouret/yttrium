
#include "y/jive/lexical/plugin.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            Plugin:: ~Plugin() noexcept
            {
            }

            Message Plugin:: onEnter(const Token &token)
            {
                return enter(token);
            }

            Message Plugin:: onLeave(const Token &token)
            {
                return leave(token);
            }

        }

    }

}

#include "y/jive/lexer.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            Scanner & Plugin:: L2S(Lexer &lexer) noexcept
            {
                return lexer;
            }


        }

    }

}
