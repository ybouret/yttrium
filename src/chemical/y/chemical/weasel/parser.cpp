
#include "y/chemical/weasel/parser.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {


            Parser:: Parser(const char * const id) : Jive::Parser(id)
            {
                Agg & CHEMICAL = agg("CHEMICAL");

                const Rule &END    = opt(mark(';'));
                const Rule &SEP    = mark(':');
                const Rule &UUID   = term("UUID", "[:alpha:][[:word:]_]*");
                const Rule &PLUS   = term('+');
                const Rule &MINUS  = term('-');
                const Rule &POS    = agg("POS") << oom(PLUS);
                const Rule &NEG    = agg("NEG") << oom(MINUS);
                const Rule &CHARGE = opt(pick(POS,NEG));


                Agg        & EQ          = agg("EQ") << UUID << SEP;
                const Agg  & SP          = agg("SP") << UUID << CHARGE << END;
                {
                    const Rule & CF          = opt(term("[:digit:]+"));
                    const Rule & ACTOR       = agg("ACTOR") << CF << mark('[') << SP << mark(']');
                    const Rule & FIRST_ACTOR = (grp("FIRST_ACTOR") << opt(PLUS) << ACTOR );
                    const Rule & EXTRA_ACTOR = (grp("EXTRA_ACTOR") << PLUS << ACTOR);
                    const Rule & ACTORS      = (grp("ACTORS") << FIRST_ACTOR << zom(EXTRA_ACTOR));
                    const Rule & OPT_ACTORS  = opt(ACTORS);
                    const Rule & REAC        = (agg("REAC") << OPT_ACTORS );
                    const Rule & PROD        = (agg("PROD") << OPT_ACTORS );
                    const Rule & CONSTANT    = plug<Jive::Lexical::RString>("K");
                    EQ << REAC << mark("<=>")  << PROD << SEP << CONSTANT << END;
                }
                const Rule &RX        = term("RX","@[^[:space:]]+");


                CHEMICAL += zom(pick(EQ,SP,RX));

                lexer.drop("[:blank:]");
                lexer.endl("[:endl:]");

                renderGraphViz();
            }

            Parser:: ~Parser() noexcept
            {
            }
        }
    }

}

