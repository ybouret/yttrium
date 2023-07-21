
//! \file

#ifndef Y_Text_Ops_Included
#define Y_Text_Ops_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! common ops on text
    //
    //__________________________________________________________________________
    struct TextOps
    {
        //! safe copy of msg into buffer
        static char *CopyMessage(char *buffer, const size_t buflen, const char *msg) noexcept;

        //! safe removal of trailing bad characters
        static char *TrimInvalid(char *buffer, const size_t buflen, const char *bad, const size_t num) noexcept;



    };

    //__________________________________________________________________________
    //
    //! type agnostic safe strlen
    //__________________________________________________________________________
    template <typename T>   inline
    size_t StringLength(const T *arr) noexcept
    {
        if(0==arr) return 0;
        else {
            size_t count = 0;
            while( 0 != *(arr++) ) ++count;
            return count;
        }
    }

}

#endif

