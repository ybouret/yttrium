
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

            void Plugin:: pushUnit(Token &content)
            {
                assert(content.size>0);
                assert(0!=content.head);
                const Entity  &ent = *this;
                const Context &ctx = *content.head;

                Unit *unit = new Unit(ent,ctx);
                unit->swapWith(content);
                lexer.put(unit);

            }

        }

    }

}
