#include "y/csv/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"

namespace Yttrium
{

    namespace CSV
    {
        Parser:: ~Parser() noexcept
        {
        }

        Parser:: Parser() : Jive::Parser("CSV")
        {

            Agg        & CSV   = agg("CSV");
            const Rule & JSTR  = plug<Jive::Lexical::JString>("JString");
            const Rule & RSTR  = plug<Jive::Lexical::RString>("RString");
            const Rule & DATA  = term("DATA","[[:alnum:][:blank:]]+");
            const Rule & FIELD = alt("FIELD") << DATA << JSTR << RSTR;

            CSV << zom(FIELD);

            lexer.endl("[:endl:]",true);
            lexer.drop("[:blank:]");
        }
    }

}
