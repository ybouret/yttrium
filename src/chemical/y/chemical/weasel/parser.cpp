
#include "y/chemical/weasel/parser.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/jive/lexical/plugin/multi-lines-comment.hpp"

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
                    const Rule & CF          = opt(term("CF","[:digit:]+"));
                    const Rule & ACTOR       = agg("ACTOR") << CF << mark('[') << SP << mark(']');
                    //const Rule & XPLUS       = term("PLUS",'+');
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
                lexer.plug<Jive::Lexical::CxxComment>("CxxComment");
                lexer.plug<Jive::Lexical::C_Comment>("C_Comment");


                renderGraphViz();

                validate();
            }

            Parser:: ~Parser() noexcept
            {
            }

            static inline
            bool IsPlus(const Jive::Syntax::XNode &node) noexcept
            {
                return  '+' == *node.rule.name;
            }

            XNode *  Parser::  load(Jive::Module *input)
            {
                Jive::Parser  &self = *this;
                AutoPtr<XNode> ast  = self(input);

                assert(ast.isValid());
                assert( *(ast->rule.name) == "CHEMICAL" );
                for(XNode *node=ast->branch().head;node;node=node->next)
                {
                    if( "EQ" != *(node->rule.name) ) continue;
                    for(XNode *sub=node->branch().head;sub;sub=sub->next)
                    {
                        const String &sid = *(sub->rule.name);
                        if( "REAC" == sid || "PROD" == sid )
                            sub->removeChildIf( IsPlus );
                    }
                }
                
                return ast.yield();
            }

        }
    }

}

