
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
            // Top Level
            //
            //------------------------------------------------------------------
            Agg &WEASEL = agg(CallSign);

            //------------------------------------------------------------------
            //
            // Whitespaces
            //
            //------------------------------------------------------------------
            const Rule &WHITE = oom(alt("WHITE") << endl("ENDL","[:endl:]",Dividing) << mark("BLANK", "[ \t]"));

            Alt &STATEMENT    = alt("STATEMENT");
            WEASEL << zom(STATEMENT);

            STATEMENT << WHITE;

            //std::cerr << "WHITE.uuid=" << FourCC::ToText(WHITE.uuid) << std::endl;

            //const Rule & ID = term("ID","[:alpha:][:word:]*");
            //STATEMENT << ID;

            //STATEMENT << WHITE;

            //------------------------------------------------------------------
            // Lexical Only
            //------------------------------------------------------------------

            (void) lexer.plug<Lingo::Lexical::CPlusPlusComment>("Comment++");
            (void) lexer.plug<Lingo::Lexical::C_Comment>("Comment");
            render();
        }


        

    }

}
