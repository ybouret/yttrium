
#include "y/ink/image/codec.hpp"

namespace Yttrium
{
    namespace Ink
    {

        Codec:: ~Codec() noexcept {}
        const char *   Codec:: callSign() const noexcept { return name.c_str(); }
        const String & Codec:: key()      const noexcept { return name; }

    }

}
