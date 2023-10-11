
#include "y/chem/lang/parser.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/jive/lexical/plugin/multi-lines-comment.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Parser:: ~Parser() noexcept
        {
        }


        Parser:: Parser() : Jive::Parser("Chemical")
        {
            Agg &TOPO = agg("topo");

            //__________________________________________________________________
            //
            //
            // species/equilibrium name
            //
            //__________________________________________________________________
            const Rule &UID     = term("UID","[:alpha:][:word:]*");
            const Rule &PLUS    = term('+');
            const Rule &MINUS   = term('-');
            const Rule &ZP      = (agg("Z+") << oom(PLUS));
            const Rule &ZM      = (agg("Z-") << oom(MINUS));
            const Rule &CHARGE  = (alt("Z") << ZP << ZM);
            const Rule &SPECIES = (agg("SPECIES") << mark('[') << UID << opt(CHARGE) << mark(']') );
            const Rule &END     = mark(';');

            //__________________________________________________________________
            //
            //
            // equilibrium
            //
            //__________________________________________________________________
            Agg        &EQ  = agg("EQ");
            {
                const Rule &SEP = mark(':');
                EQ << UID << SEP;
                const Rule &COEFF  = term("COEFF","[1-9][0-9]*");
                const Rule &ACTOR  = (agg("ACTOR") << opt(COEFF) << SPECIES);
                const Rule &ACTORS = zom(grp("ACTORS") << ACTOR << zom( cat(PLUS,ACTOR) ));
                const Rule &REAC   = agg("REAC") << ACTORS;
                const Rule &PROD   = agg("PROD") << ACTORS;
                EQ << REAC << mark("<=>") << PROD;
            }

            const Rule &ITEM = alt("ITEM") << SPECIES << EQ;
            const Rule &DECL = grp("DECL") << ITEM << opt(END);
            TOPO << zom(DECL);

            renderGraphViz(true);

            // lexical only
            lexer.plug<Jive::Lexical::C_Comment>("COM1");
            lexer.plug<Jive::Lexical::CxxComment>("COM2");
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");

            validate();
        }



    }
}
