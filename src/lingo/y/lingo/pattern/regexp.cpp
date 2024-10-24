#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/text/plural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        static inline
        Pattern * _RegExp(const char * const       expr,
                          const size_t             size,
                          const Dictionary * const dict)
        {
            assert(0!=expr);
            RXC    rxc(expr,size,dict);
            AutoPtr<Pattern> p =  rxc.subExpr(); assert(p.isValid());

            if(rxc.depth!=0)
                throw Specific::Exception(RXC::CallSign,"missing %d parenthese%s in '%s'",rxc.depth,Plural::s(rxc.depth),expr);

            return Pattern::Optimize(p.yield());
        }

        Pattern * RegExp(const String     &       expr,
                         const Dictionary * const dict)
        {
            return _RegExp(expr.c_str(), expr.size(), dict);
        }

        Pattern * RegExp(const char * const expr, const Dictionary * const dict)
        {
            const  String _(expr);
            return RegExp(_,dict);
        }

    }
}
