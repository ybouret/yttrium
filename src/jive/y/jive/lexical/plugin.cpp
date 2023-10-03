
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
