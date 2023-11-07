
#include "y/system/hw.hpp"
#include "y/system/exception.hpp"

extern "C"
{
    size_t Yttrium_Hardware_NumProcs();
}

namespace Yttrium
{
    size_t Hardware::NumProcs()
    {
        const size_t np = Yttrium_Hardware_NumProcs();
        if(np<=0) throw Specific::Exception("Hardware::NumProcs","No CPU Found!");
        return np;
    }
}

