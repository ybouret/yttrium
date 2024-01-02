
//! \file

#ifndef Y_CXX_Identifier_Included
#define Y_CXX_Identifier_Included 1

#include "y/string.hpp"


namespace Yttrium
{

    //! creating C++ identifier from string
    struct CxxIdentifier
    {

        static String From(const String &); //!< create from string
        static String From(const char   *); //!< create from text
    };

}

#endif
