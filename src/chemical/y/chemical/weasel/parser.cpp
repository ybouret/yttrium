
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
            WEASEL << zom(STATEMENT);

            //------------------------------------------------------------------
            //
            // Whitespaces
            //
            //------------------------------------------------------------------
            const Rule &WHITE = alt("WHITE") << endl("ENDL","[:endl:]",Dividing) << mark("BLANK", "[ \t]");
            STATEMENT << oom(WHITE);

            //const Rule &SPACE = opt(WHITE);
            const Rule &NAME = term("NAME","[:upper:][[:lower:]_]*");
            const Rule &COEF = term("COEF","[:digit:]+");
            STATEMENT << NAME;
            STATEMENT << COEF;


            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }


        

    }

}
