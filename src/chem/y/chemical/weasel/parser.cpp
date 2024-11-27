#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"
#include "y/lingo/lexical/add-on/rstring.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Weasel:: Parser:: ~Parser() noexcept
        {
        }


        static const char * actorsTable[]
        {
            "REAC",
            "PROD"
        };




        Weasel:: Parser:: Parser(const Lingo::Caption &caption) :
        Lingo::Parser(caption),
        WEASEL(      agg("WEASEL")      ), // top-level rule
        FORMULA(     agg("FORMULA")     ),
        EQUILIBRIUM( agg("EQUILIBRIUM") ),
        POSITIVE(    term('+')          ),
        actors( Y_Hashing_Perfect_Table(actorsTable) )
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
            EQUILIBRIUM << EQ << WHITE << COLON;
            Compound   &ACTOR  = agg("ACTOR")  << WHITE << OPT_INT << WHITE << FORMULA;
            Compound   &ACTORS = grp("ACTORS") << ACTOR << zom( cat(WHITE,POSITIVE,ACTOR) );
            const Rule &ACTORX = opt(ACTORS);
            EQUILIBRIUM << (agg("REAC") << ACTORX);
            EQUILIBRIUM << WHITE << mark( SYMBOL );
            EQUILIBRIUM << WHITE << (agg("PROD") << ACTORX);
            EQUILIBRIUM << WHITE << COLON;
            EQUILIBRIUM << WHITE << plug<Lingo::Lexical::RString>("K");


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


        static inline
        void removeFrom(XNode * const node, const Lingo::Syntax::Rule &rule) noexcept
        {
            assert( 0 != node );
            XList &source = node->branch();
            XList  target;
            {
                const Lingo::Syntax::Rule * const lhs = &rule;
                while(source.size>0)
                {
                    AutoPtr<XNode>                    sub = source.popHead();
                    const Lingo::Syntax::Rule * const rhs = & (sub->rule);
                    if( lhs == rhs ) continue;
                    target.pushTail( sub.yield() );
                }
            }
            source.swapWith(target);
        }

        XNode * Weasel:: Parser:: postProcess(XNode * const root) const noexcept
        {
            assert(0!=root);
            assert(root->name() == *WEASEL.name );
            XList &st = root->branch();
            for(XNode *node=st.head;node;node=node->next)
            {
                if(node->name() != *EQUILIBRIUM.name) continue;
                for(XNode *sub=node->branch().head;sub;sub=sub->next)
                {
                    switch( actors(sub->name()) )
                    {
                        case Y_Weasel_REAC:
                            removeFrom(sub,POSITIVE);
                            break;

                        case Y_Weasel_PROD:
                            removeFrom(sub,POSITIVE);
                            break;
                    }
                }
            }

            return root;
        }


    }

}

