
//! \file

#ifndef Y_CXX_Identifier_Included
#define Y_CXX_Identifier_Included 1

#include "y/string.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! creating C++ identifier from string
    //
    //__________________________________________________________________________
    struct CxxIdentifier
    {
        //______________________________________________________________________
        //
        //! style of output
        //______________________________________________________________________
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
