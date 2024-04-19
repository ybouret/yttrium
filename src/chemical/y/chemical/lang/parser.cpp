#include "y/chemical/lang/parser.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Parser:: ~Parser() noexcept
        {
        }

        Parser:: Parser(const char *id) : Jive::Parser(id)
        {
            Agg       &CHEMICAL   = agg("CHEMICAL"); // top-level

            const Rule &PLUS    = term('+');
            const Rule &MINUS   = term('-');
            const Rule &UUID    = term("UUID","[:alpha:][:word:]*");
            const Rule &ZPOS    = (agg("ZPOS") << oom(PLUS));
            const Rule &ZNEG    = (agg("ZNEG") << oom(MINUS));
            const Rule &SPECIES = (agg("SPECIES") << mark('[') << UUID << opt( pick(ZPOS,ZNEG) ) << mark(']'));

            const Rule &COEF        = term("COEF","[:digit:]+");
            const Rule &OPT_COEF    = opt(COEF);
            const Rule &ACTOR       = (agg("ACTOR") << OPT_COEF << SPECIES);

            const Rule &FIRST_ACTOR = (grp("FIRST_ACTOR") << opt(PLUS) << ACTOR );
            const Rule &EXTRA_ACTOR = (grp("EXTRA_ACTOR") << PLUS << ACTOR);
            const Rule &ACTORS      = (grp("ACTORS") << FIRST_ACTOR << zom(EXTRA_ACTOR));
            const Rule &OPT_ACTORS  = opt(ACTORS);
            const Rule &REAC        = (agg("REAC") << OPT_ACTORS );
            const Rule &PROD        = (agg("PROD") << OPT_ACTORS );
            const Rule &CONSTANT    = plug<Jive::Lexical::RString>("K");
            const Rule &SEP         = mark(':');
            const Rule &EQ          = (agg("EQ") << UUID << SEP << REAC << mark("<=>")  << PROD << SEP << CONSTANT) ;

            CHEMICAL += zom(pick(SPECIES,EQ));


            renderGraphViz();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");
        }
        

    }

}
