#include "y/lingo/lexical/add-on.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            
            AddOn:: ~AddOn() noexcept
            {
            }

            void AddOn:: onLeave(const Token &token)
            {
            }

            void AddOn:: onEnter(const Token &token)
            {

            }

        }

    }

}

#include "y/lingo/lexer.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            void AddOn:: trigger(Lexer &      lexer,
                                 Rule * const rule)
            {
                assert(0!=rule);
                lexer.add(rule);
            }

        }

    }

}

