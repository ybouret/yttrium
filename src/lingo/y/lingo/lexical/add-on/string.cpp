
#include "y/lingo/lexical/add-on/string.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            String_:: ~String_() noexcept
            {
            }

            void String_:: enter(const Token &token)
            {
                content.release();
                content += token;
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
            void String_:: leave(const Token &token)
            {
                content += token;
                lexer.put( produce(content) );
            }
        }

    }

}

