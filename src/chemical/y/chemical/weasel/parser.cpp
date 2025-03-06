
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"
#include "y/lingo/lexical/add-on/rstring.hpp"

#include "y/system/exception.hpp"
#include <iomanip>

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
            const Rule &SIGN      = pick(PLUS,MINUS);
            const Rule &SPACE     = zom(WHITE);
            const Rule &STRING    = plug<Lingo::Lexical::RString>("String");


            //------------------------------------------------------------------
            //
            // Create Formula
            //
            //------------------------------------------------------------------
            {
                const Rule &NAME     = term("Name","[:upper:][[:lower:]_]*");
                Agg        &GROUP    = act("Group");
                const Rule &BODY     = act("Body") << GROUP << zom(GROUP);
                Alt        &ITEM     = alt("Item");
                GROUP   << ITEM     << OPT_COEF;
                ITEM    << NAME     << parens(BODY);
                const Rule & CHARGE  = agg("Charge") << '^' << OPT_COEF << SIGN;
                FORMULA << BODY << opt(CHARGE);
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

                const Rule & ACTOR  = agg("Actor")  << SPACE << OPT_COEF << SPACE << FORMULA << SPACE;
                const Rule & ACTORS = grp("Actors") << ACTOR << extra('+',ACTOR);
                const Rule & EQSIDE = pick(ACTORS,SPACE);
                EQUILIBRIUM << (agg("Reac") << EQSIDE);
                EQUILIBRIUM << SPACE << mark(Equilibrium::Symbol);
                EQUILIBRIUM << (agg("Prod") << EQSIDE);

                EQUILIBRIUM << SPACE << EQSEP;
                EQUILIBRIUM << SPACE << STRING;
            }

            STATEMENT << EQUILIBRIUM;

            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }





        XNode * Weasel::Parser:: preprocess(Lingo::Module * const inputModule)
        {
            Lingo::Parser &self = *this;
            AutoPtr<XNode> ast  = self(inputModule);

            assert( CallSign == ast->name() );



            return ast.yield();
        }


        

    }

}
