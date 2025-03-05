
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
            Agg &WEASEL = agg(CallSign);

            const Rule &ENDL  = endl("ENDL","[:endl:]",Dividing);
            const Rule &BLANK = mark("BLANK", "[ \t]");
            const Rule &WHITE = zom(alt("WHITE") << ENDL << BLANK);

            WEASEL << WHITE;


            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }


        

    }

}
