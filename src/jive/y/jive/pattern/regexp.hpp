
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

        //______________________________________________________________________
        //
        //
        //! Regular Expressions interface
        //
        //______________________________________________________________________
        struct RegExp
        {
            static Pattern *Compile(const String &rx, const Dictionary *dict =0); //!< compile regular expression
            static Pattern *Compile(const char   *rx, const Dictionary *dict =0); //!< compuler rgular expression
        };
    }

}

#endif

