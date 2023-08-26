
//! \file

#ifndef Y_Text_Convert_Included
#define Y_Text_Convert_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{

    namespace Core { template <typename> class String; }

    namespace ASCII
    {
        //______________________________________________________________________
        //
        //
        //! string to data
        //
        //______________________________________________________________________
        struct Convert
        {
            //__________________________________________________________________
            //
            //! convert to uint64_t, whith overflow control, hex/dec
            //__________________________________________________________________
            static uint64_t ToU64(const char *msg, size_t len, const char *ctx=0);
            static uint64_t ToU64(const char *msg, const char *ctx=0);               //!< alias
            static uint64_t ToU64(const Core::String<char> &msg, const char *ctx=0); //!< alias

            //__________________________________________________________________
            //
            //! signed decimal
            //__________________________________________________________________
            static int64_t  ToI64(const char *msg, size_t len, const char *ctx=0);
            static int64_t  ToI64(const char *msg, const char *ctx=0);               //!< alias
            static int64_t  ToI64(const Core::String<char> &msg, const char *ctx=0); //!< alias

        };
    }

}

#endif


