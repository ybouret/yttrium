
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
        
        Syntax::XNode * Parser:: operator()(Module * const m)
        {
            assert(0!=m);
            Source source(m);
            return  (*this)(source);
        }
        
#define Y_Lingo_Parser_Query(expr)                   \
/**/  do  {                                          \
/**/    const Grammar &    self = *this;             \
/**/    const Rule * const rule = self->query(expr); \
/**/    if(0!=rule) return *rule;                    \
/**/  } while(false)

        const Syntax::Rule & Parser :: get(const String &expr)
        {
            Y_Lingo_Parser_Query(expr);
            return term(expr);
        }

        const Syntax::Rule & Parser :: get(const char * const expr)
        {
            Y_Lingo_Parser_Query(expr);
            return term(expr);
        }

        const Syntax::Rule & Parser :: get(const char expr)
        {
            Y_Lingo_Parser_Query(expr);
            return mark(expr);
        }


    }

}
