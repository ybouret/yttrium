
#include "y/system/hw.hpp"

extern "C"
{
    size_t Yttrium_Hardware_NumProcs();
}

namespace Yttrium
{
    size_t Hardware::NumProcs()
    {
        return Yttrium_Hardware_NumProcs();
    }
}

