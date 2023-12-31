
//! \file

#ifndef Y_Text_ASCII_Embedding_Included
#define Y_Text_ASCII_Embedding_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{
    namespace ASCII
    {
        //______________________________________________________________________
        //
        //
        //
        //! escaped chars to write labels for Doxygen of C code
        //
        //
        //______________________________________________________________________
        struct Embedding
        {
            static const char * const Char[256]; //!< table of substitution
        };

    }
}

#endif

