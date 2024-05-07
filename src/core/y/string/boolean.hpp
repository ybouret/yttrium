
//! \file

#ifndef Y_String_Tokenizer_Included
#define Y_String_Tokenizer_Included 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    struct StringToBoolean
    {
        static bool MeansTrue(const char  * const lower) noexcept;
        static bool MeansFalse(const char * const lower) noexcept;
        static bool Get(const char *buffer, const size_t buflen, const char *field);
        static bool Get(const Core::String<char> &name, const char *field);
    };

}

#endif
