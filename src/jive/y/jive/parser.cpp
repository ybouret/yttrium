#include "y/jive/parser.hpp"
#include "y/jive/source.hpp"
#include "y/system/exception.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Parser:: ~Parser() noexcept
        {
        }

        Syntax::XNode * Parser:: operator()(Jive::Module *m)
        {
            Source source(m);
            lexer.set();
            return parse(lexer,source);
        }

        

    }

}


