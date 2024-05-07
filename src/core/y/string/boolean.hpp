
//! \file

#ifndef Y_String_Tokenizer_Included
#define Y_String_Tokenizer_Included 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    //! converting string to boolean
    struct StringToBoolean
    {
        static bool MeansTrue(const char  * const lower) noexcept;                   //!< "1" or "on" of "true"
        static bool MeansFalse(const char * const lower) noexcept;                   //!< "0" or "off" or "false"
        static bool Get(const char *buffer, const size_t buflen, const char *field); //!< convert or throw
        static bool Get(const Core::String<char> &name, const char *field);          //!< convert or throw
    };

}

#endif
