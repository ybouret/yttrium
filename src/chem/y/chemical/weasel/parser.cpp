#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Weasel:: Parser:: ~Parser() noexcept
        {
        }

        Weasel:: Parser:: Parser(const Lingo::Caption &caption) : Lingo::Parser(caption)
        {

            Agg        &WEASEL   = agg("WEASEL");

            const Rule & ELEMENT  = term("ELEMENT","[:upper:][:lower:]*");
            const Rule & INTEGER  = term("INTEGER","[:digit:]+");
            const Rule & OPT_INT  = opt(INTEGER);
            Compound   & STOCHIO  = act("STOCHIO");
            Compound   & SPECIES  = agg("SPECIES");
            Compound   & CONTENT  = alt("CONTENT");
            const Rule & POSITIVE = term('+');
            const Rule & NEGATIVE = term('-');
            const Rule & SIGN     = alt("SIGN") << POSITIVE << NEGATIVE;
            const Rule & Z        = agg("Z") << '^' << OPT_INT << SIGN;

            SPECIES << STOCHIO <<    zom(STOCHIO);
            STOCHIO << CONTENT <<       (OPT_INT);
            CONTENT << ELEMENT << parens(SPECIES);
            SPECIES << opt(Z);


            const Rule & BLANK   = mark("[:blank:]");
            const Rule & ENDL    = endl("[:endl:]",Dividing);
            const Rule & SPACE   = alt("SPACE") << BLANK << ENDL;
            const Rule & SPACES  = oom(SPACE);
            const Rule & WHITE   = opt(SPACES);

            const Rule & DECL = agg("DECL") << WHITE << SPECIES << WHITE;
            WEASEL << zom(DECL);

            lexer.plug<Lingo::Lexical::C_Comment>("C_Comment");
            lexer.plug<Lingo::Lexical::CPlusPlusComment>("C++Comment");

            render();

        }

    }

}

