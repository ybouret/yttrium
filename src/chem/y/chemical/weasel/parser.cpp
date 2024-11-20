#include "y/chemical/weasel/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Weasel:: Parser:: ~Parser() noexcept
        {
        }

        Weasel:: Parser:: Parser(const Lingo::Caption &caption) : Lingo::Parser(caption)
        {

            Alt &        WEASEL = alt(name);
            const Rule & SYMBOL = term("SYMBOL","[:alpha:][[:word:]\\(\\)]*(^)?");

            WEASEL << zom(SYMBOL);

            render();
            validate();
            
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]",Lingo::Lexeme::Drop);
        }

    }

}

