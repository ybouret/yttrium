
//! \file

#ifndef Y_String_Included
#define Y_String_Included 1

#include "y/string/string.hpp"
#include "y/check/printf.hpp"

namespace Yttrium
{
    typedef Core::String<char> String; //!< default string
    
    //! Formatted String API
    struct Formatted
    {
        //! string size from format and va_list address
        static int ComputeSize(const char *fmt, void *ptr) noexcept;
        
        //! write into string with format and va_list address
        static int WriteString(String &ans, const char * fmt, void *ptr) noexcept;


        //! C-style formatting
        static String  Get(const char *fmt,...) Y_PRINTF_CHECK(1,2);

        //! C-style formatting
        static String *New(const char *fmt,...) Y_PRINTF_CHECK(1,2);
    };


}



#endif

