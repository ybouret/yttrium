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

        const Syntax::Rule & Parser:: term_(const Lexical::Action &action, const Term::Property &ppty)
        {
            return add( new Term(action.name,ppty,action.motif->isUnivocal()) );
        }

        

    }

}


