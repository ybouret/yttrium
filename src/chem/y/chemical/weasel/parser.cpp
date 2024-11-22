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

            Agg  & ADD   = agg("ADD");
            Agg  & POW   = act("POW");
            Alt  & ATOM  = alt("ATOM");
            ADD << POW << zom(POW);
            POW << ATOM << opt( cat(get('_'), term("COEF","[:digit:]+" )) );
            ATOM << term("ID","[:upper:][:lower:]*") << parens(ADD);

#if 0
            const Rule &COEF   = term("COEF","[:digit:]+");
            Compound   &ATOM   = alt("ATOM") << term("name","[:alpha:]+");
            Compound   &BODY   = agg("BODY") << ATOM << zom(ATOM);
            ATOM << parens(BODY);



            WEASEL << zom(SPECIES);
#endif
            const Rule & BLANK   = mark("[:blank:]");
            const Rule & ENDL    = endl("[:endl:]",Dividing);
            const Rule & SPACE   = alt("SPACE") << BLANK << ENDL;
            const Rule & SPACES  = oom(SPACE);
            const Rule & WHITE   = opt(SPACES);

            const Rule & SPECIES = agg("SPECIES") << WHITE << ADD << WHITE;
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

