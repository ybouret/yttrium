
#include "y/jive/pattern/regexp.hpp"
#include "y/jive/pattern/regexp/compiler.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Pattern *RegExp:: Compile(const String &rx, const Dictionary *dict)
        {
            return RegExpCompiler::Instance().compile(rx,dict);
        }

        Pattern *RegExp:: Compile(const char *rx, const Dictionary *dict)
        {
            const String _(rx);
            return Compile(_,dict);
        }
    }

}
