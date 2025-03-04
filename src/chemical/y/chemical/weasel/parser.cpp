
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            Parser:: ~Parser() noexcept
            {
            }

            Parser:: Parser() : Lingo::Parser("Weasel")
            {
                
                (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
                (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            }


        }

    }

}
