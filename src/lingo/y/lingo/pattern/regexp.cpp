#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/pattern/regexp/compiler.hpp"

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
            return rxc.subExpr();
        }

        Pattern * RegExp(const String     &       expr,
                         const Dictionary * const dict)
        {
            std::cerr << "Compiling '" << expr << "'" << std::endl;
            return _RegExp(expr.c_str(), expr.size(), dict);
        }

        Pattern * RegExp(const char * const expr, const Dictionary * const dict)
        {
            const String _(expr);
            return RegExp(_,dict);
        }

    }
}
