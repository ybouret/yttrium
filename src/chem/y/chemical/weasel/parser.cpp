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

            const Rule &ELEMENT  = term("ELEMENT","[:upper:][:lower:]*");
            const Rule &INTEGER  = term("INTEGER","[:digit:]+");

            Compound  & ADD  = agg("ADD");
            Compound  & MUL  = agg("MUL");
            Compound  & ATOM = alt("ATOM");
            ADD << MUL << zom(MUL);
            MUL << ATOM << opt(INTEGER);
            ATOM << ELEMENT << parens(ADD);
            //ATOM << ELEMENT << cat( term('('), ADD, term(')') );

            const Rule & BLANK   = mark("[:blank:]");
            const Rule & ENDL    = endl("[:endl:]",Dividing);
            const Rule & SPACE   = alt("SPACE") << BLANK << ENDL;
            const Rule & SPACES  = oom(SPACE);
            const Rule & WHITE   = opt(SPACES);

            const Rule & SPECIES = agg("SPECIES") << WHITE << ADD << WHITE;
            WEASEL << zom(SPECIES);

            lexer.plug<Lingo::Lexical::C_Comment>("C_Comment");
            lexer.plug<Lingo::Lexical::CPlusPlusComment>("C++Comment");

            render();

#
        }

    }

}

