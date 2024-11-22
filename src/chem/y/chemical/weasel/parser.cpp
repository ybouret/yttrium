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

            Agg        &WEASEL = agg("WEASEL");
            const Rule &COEF   = term("COEF","[:digit:]+");
            const Rule &STOI   = opt(COEF);
            Compound   &ATOM   = alt("ATOM") << term("name","[:alpha:]+");
            Compound   &BODY   = agg("BODY") << ATOM << zom(ATOM) << STOI;
            ATOM << parens(BODY);

            const Rule &BLANK  = mark("[:blank:]");
            const Rule &ENDL   = endl("[:endl:]",Dividing);
            const Rule &SPACE  = alt("SPACE") << BLANK << ENDL;
            const Rule &SPACES = oom(SPACE);
            const Rule &WHITE  = opt(SPACES);

            const Rule & SPECIES = agg("SPECIES") << WHITE << BODY << WHITE;

            WEASEL << zom(SPECIES);

            render();

#if 0
            Alt &        WEASEL = alt(name);
            const Rule & SYMBOL = term("SYMBOL","[:alpha:][[:word:]\\(\\)]*(^)?");

            WEASEL << zom(SYMBOL);

            render();
            //validate();

            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]",Lingo::Lexeme::Drop);
#endif
        }

    }

}

