//! file

#ifndef Y_Lingo_RegExp_Included
#define Y_Lingo_RegExp_Included 1

#include "y/lingo/pattern/dictionary.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Pattern * RegExp(const String &expr, const Dictionary * const dict);
        
    }
}

#endif

