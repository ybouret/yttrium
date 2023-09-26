#include "y/jive/lexical/action.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            Action:: ~Action() noexcept
            {

            }

            Message Action:: perform()
            {
                const Token &token = *motif;
                assert(token.size>0);
                return doing(token);
            }

        }
    }

}
