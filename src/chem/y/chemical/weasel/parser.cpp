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
            Alt &        ATOM   = alt("ATOM");
            Compound &   ADD    = agg("ADD") << ATOM << zom(ATOM);
            const Rule & GRP    = agg("GRP") << '(' <<  ADD << ')';
            ATOM << term("ID","[:alpha:]+") << GRP;

            WEASEL << ADD;

            render();
            validate();
            
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]",Lingo::Lexeme::Drop);
        }

    }

}

