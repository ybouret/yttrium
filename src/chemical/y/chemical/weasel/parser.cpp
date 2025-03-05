
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
            const Rule &SIGN      = alt("Sign") << PLUS << MINUS;

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
                FORMULA << MOLECULE << opt(act("Charge") << '^' << OPT_COEF << SIGN);
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
                EQUILIBRIUM << LABEL << EQSEP;
            }

            STATEMENT << EQUILIBRIUM;

            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }


        

    }

}
