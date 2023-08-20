
//! \file

#ifndef Y_Jive_Pattern_RegExp_Included
#define Y_Jive_Pattern_RegExp_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Jive
    {
        class Pattern;
        class Dictionary;

        struct RegExp
        {
            static Pattern *Compile(const String &rx, const Dictionary *dict =0);
            static Pattern *Compile(const char   *rx, const Dictionary *dict =0);
        };
    }

}

#endif

