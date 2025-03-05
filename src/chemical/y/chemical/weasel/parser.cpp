
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Weasel::Parser:: ~Parser() noexcept
        {
        }


        Weasel:: Parser:: Parser() : Lingo::Parser("Weasel")
        {
            //------------------------------------------------------------------
            //
            // Top Level will always be Weasel
            //
            //------------------------------------------------------------------
            Agg &WEASEL    = agg(CallSign);
            Alt &STATEMENT = alt("STATEMENT");

            //------------------------------------------------------------------
            //
            // Whitespaces
            //
            //------------------------------------------------------------------
            const Rule &WHITE     = alt("WHITE") << endl("ENDL","[:endl:]",Dividing) << mark("BLANK", "[ \t]");
            const Rule &SEPARATOR = mark(';');
            const Rule &OPT_SEP   = opt(SEPARATOR);
            WEASEL << zom(STATEMENT);

            STATEMENT << SEPARATOR;                // single separator
            STATEMENT << cat(oom(WHITE),OPT_SEP);  // blank line

            //------------------------------------------------------------------
            //
            // Common rules for Equilibria/Formula
            //
            //------------------------------------------------------------------
            const Rule &COEF     = term("Coef","[:digit:]+");
            const Rule &OPT_COEF = opt(COEF);
            Agg        &FORMULA   = agg(Formula::CallSign);
            const Rule &PLUS      = term('+');
            const Rule &MINUS     = term('-');
            const Rule &SPACE     = zom(WHITE);

            //------------------------------------------------------------------
            //
            // Create Formula
            //
            //------------------------------------------------------------------
            {
                const Rule &NAME     = term("Name","[:upper:][[:lower:]_]*");
                Agg        &GROUP    = act("Group");
                const Rule &MOLECULE = act("Molecule") << GROUP << zom(GROUP);
                Alt        &ITEM     = alt("Item");
                GROUP   << ITEM     << OPT_COEF;
                ITEM    << NAME     << parens(MOLECULE);
                const Rule &POSITIVE = agg(Formula::Positive) << '^' << OPT_COEF << PLUS;
                const Rule &NEGATIVE = agg(Formula::Negative) << '^' << OPT_COEF << MINUS;

                FORMULA << MOLECULE << opt(alt("Charge") << POSITIVE << NEGATIVE);
            }

            STATEMENT << FORMULA;

            //------------------------------------------------------------------
            //
            // Create Equilibrium
            //
            //------------------------------------------------------------------
            Agg &EQUILIBRIUM = agg(Equilibrium::CallSign);
            {
                String rx = Equilibrium::Prefix; rx +="[[:word:]_\\(\\))]+";
                const Rule &LABEL = term("Label",rx);
                const Rule &EQSEP = mark(Equilibrium::Separator);
                EQUILIBRIUM << LABEL;
                EQUILIBRIUM << SPACE << EQSEP;
#if 1
                const Rule &ACTOR  = agg("Actor")  << SPACE << OPT_COEF << SPACE << FORMULA;
                const Rule &ACTORS = agg("Actors") << ACTOR << extra('+', ACTOR);
                const Rule &EQSIDE = pick(ACTORS,SPACE);
                EQUILIBRIUM << (agg("Reac") << EQSIDE);
                EQUILIBRIUM << SPACE << mark(Equilibrium::Symbol);
                EQUILIBRIUM << (agg("Prod") << EQSIDE);
                EQUILIBRIUM << SPACE << EQSEP;


#endif
            }

            STATEMENT << EQUILIBRIUM;

            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }


        static inline void cleanFormula(XNode &node) noexcept
        {
            XList &list = node.branch();
            if(list.size<=1) return;
            std::cerr << "Need To Clean Charge" << std::endl;
            XNode &charge = *(list.tail);
            assert( Formula::Positive == charge.name() || Formula::Negative == charge.name() );
            XList &content = charge.branch(); assert(content.size>=1);
            if(2==content.size) delete content.popTail();
            assert(1==content.size);
            XNode &coef = *content.head;
            const Lingo::Lexeme &lx = coef.lexeme();
            if(1==lx.size && '1' == **lx.head)
                content.release();
        }

        XNode * Weasel::Parser:: preprocess(Lingo::Module * const inputModule)
        {
            Lingo::Parser &self = *this;
            AutoPtr<XNode> ast  = self(inputModule);

            assert( CallSign == ast->name() );



            for(XNode *node=ast->branch().head;node;node=node->next)
            {
                if( Formula::CallSign == node->name() )
                {
                    cleanFormula(*node);
                }
            }

            return ast.yield();
        }


        

    }

}
