
#include "y/chem/lang/parser.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/jive/lexical/plugin/multi-lines-comment.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Parser:: ~Parser() noexcept
        {
        }


        Parser:: Parser() : Jive::Parser("Chemical"),
        reac("REAC"),
        prod("PROD"),
        plus("+")
        {
            // top level
            Agg &TOPO = agg(name);

            //__________________________________________________________________
            //
            //
            // species
            //
            //__________________________________________________________________
            const Rule &UID     = term("UID","[:alpha:][:word:]*");
            const Rule &PLUS    = term((*plus)[1]);
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
                const Rule &REAC   = agg(reac) << ACTORS;
                const Rule &PROD   = agg(prod) << ACTORS;
                EQ << REAC << mark("<=>") << PROD << SEP << plug<Jive::Lexical::RString>("K");
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

        Parser::XNode * Parser:: prune(XNode *root) noexcept
        {
            assert(0!=root);
            scrub(root);
            return root;
        }

       void Parser:: scrub(XNode *root) noexcept
        {
            switch(root->type)
            {
                case Jive::Syntax::IsTerminal: return;
                case Jive::Syntax::IsInternal:
                    break;
            }
            const Jive::Syntax::Rule &rule = root->rule;
            if( *(rule.name) == *reac || *(rule.name) == *prod )
            {
                root->remove(plus);

            }
            else
            {
                for(XNode *node=root->branch().head;node;node=node->next)
                    scrub(node);
            }
        }


    }
}
