
//! file


#ifndef Y_Text_C_Format_Included
#define Y_Text_C_Format_Included 1


#include "y/config/starting.hpp"

namespace Yttrium
{

    //! helper to use C format API
    struct C_Format
    {
        //! format using ptr = & va_list
        static void Buffer(char * const buffer,
                           const size_t buflen,
                           const char * const fmt,
                           void * const       ptr) noexcept;
    };

}

#endif
