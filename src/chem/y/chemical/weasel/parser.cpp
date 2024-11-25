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



        Weasel:: Parser:: Parser(const Lingo::Caption &caption) :
        Lingo::Parser(caption),
        WEASEL(  agg("WEASEL")   ), // top-level rule
        FORMULA( agg("FORMULA")  )
        {
            //------------------------------------------------------------------
            //
            //
            // handling whitespace
            //
            //
            //------------------------------------------------------------------
            const Rule & BLANK   = mark("[:blank:]");
            const Rule & ENDL    = endl("[:endl:]",Dividing);
            const Rule & SPACE   = alt("SPACE") << BLANK << ENDL;
            const Rule & SPACES  = oom(SPACE);
            const Rule & WHITE   = opt(SPACES);

            //------------------------------------------------------------------
            //
            //
            // building recursive formula
            //
            //
            //------------------------------------------------------------------
            const Rule & ELEMENT  = term("ELEMENT","[:upper:][:lower:]*");
            const Rule & INTEGER  = term("INTEGER","[:digit:]+");
            const Rule & OPT_INT  = opt(INTEGER);
            Compound   & STOCHIO  = act("STOCHIO");
            Compound   & CONTENT  = alt("CONTENT");
            const Rule & POSITIVE = term('+');
            const Rule & NEGATIVE = term('-');
            const Rule & SIGN     = alt("SIGN") << POSITIVE << NEGATIVE;
            const Rule & Z        = agg("Z") << '^' << OPT_INT << SIGN;

            FORMULA << STOCHIO <<    zom(STOCHIO);
            STOCHIO << CONTENT <<       (OPT_INT);
            CONTENT << ELEMENT << parens(FORMULA);
            FORMULA << opt(Z);


            //------------------------------------------------------------------
            //
            //
            // building Equilibrium
            //
            //
            //------------------------------------------------------------------
            const Rule &EQ          = term("EQ","@[:word:]+");
            Agg        &EQUILIBRIUM = agg("EQUILIBRIUM");
            EQUILIBRIUM << EQ << WHITE << ':';
            Compound   &ACTOR  = agg("ACTOR")  << WHITE << OPT_INT << WHITE << FORMULA;
            Compound   &ACTORS = grp("ACTORS") << ACTOR << zom( cat(WHITE,POSITIVE,ACTOR) );
            const Rule &ACTORX = opt(ACTORS);
            EQUILIBRIUM << (agg("REAC") << ACTORX);
            EQUILIBRIUM << WHITE << mark( Weasel::EqSep );
            EQUILIBRIUM << WHITE << (agg("PROD") << ACTORX);



            //------------------------------------------------------------------
            //
            //
            // declaring statements
            //
            //
            //------------------------------------------------------------------
            Compound & STATEMENT = alt("STATEMENT");
            STATEMENT <<  EQUILIBRIUM << FORMULA;

            const Rule &SEP = opt( get(';') );
            WEASEL << zom( cat(WHITE,STATEMENT,WHITE,SEP) ) << WHITE;


            // lexer only: comments
            lexer.plug<Lingo::Lexical::C_Comment>("C_Comment");
            lexer.plug<Lingo::Lexical::CPlusPlusComment>("C++Comment");

            render();
            //Rule::Trace = true;
        }


        

    }

}

