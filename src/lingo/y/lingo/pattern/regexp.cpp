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
            RXC    rxc(expr,size,dict);
            return 0;
        }

        Pattern * RegExp(const String     &       expr,
                         const Dictionary * const dict)
        {
            return _RegExp(expr.c_str(), expr.size(), dict);
        }

    }
}
