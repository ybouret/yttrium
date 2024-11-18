
#include "y/lingo/parser.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Parser:: ~Parser() noexcept
        {

        }


        Syntax::XNode * Parser:: operator()(Source &source)
        {
            lexer.restart();
            XNode * const cst = accept(lexer,source);
            return XNode::AST(cst);
        }


        const Syntax::Rule & Parser:: get(const String &expr)
        {
            {
                const Rule * const rule = (*this)->query(expr);
                if(0!=rule) return *rule;
            }
            return term(expr);
        }

        const Syntax::Rule & Parser:: get(const char * const expr)
        {
            const String _(expr);
            return get(_);
        }

        const Syntax::Rule & Parser:: get(const char c)
        {
            {
                const Rule * const rule = (*this)->query(c);
                if(0!=rule) return *rule;
            }
            return mark(c);
        }




    }

}
