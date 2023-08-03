
#include "y/associative/key-variety.hpp"

namespace Yttrium
{

#define Y_KV(TYPE) case TYPE: return #TYPE

    const char * KeyVariety:: Type2Text(const Type t) noexcept
    {
        switch(t)
        {
                Y_KV(IsNotAvailable);
                Y_KV(IsMemoryBuffer);
                Y_KV(IsIntegralType);
                Y_KV(IsLegacyString);
        }
        return Core::Unknown;
    }

}
