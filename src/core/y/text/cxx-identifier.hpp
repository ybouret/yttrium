
//! \file

#ifndef Y_CXX_Identifier_Included
#define Y_CXX_Identifier_Included 1

#include "y/string.hpp"


namespace Yttrium
{

    //! creating C++ identifier from string
    struct CxxIdentifier
    {
        enum Style
        {
            Compact,     //!< remove trailing '_' and multiple '__'
            Verbatim     //!< full conversion
        };
        static String From(const String &, const Style = Compact); //!< create from string
        static String From(const char   *, const Style = Compact); //!< create from text
    };

}

#endif
