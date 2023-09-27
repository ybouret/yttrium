
#include "y/type/static-retainer.hpp"

namespace Yttrium
{
    StaticRetainer:: StaticRetainer(Counted &obj) noexcept :
    host(obj)
    {
        host.withhold();
        assert(host.quantity()>=1);
    }

    StaticRetainer:: ~StaticRetainer() noexcept
    {
        assert(host.quantity()>=1);
        (void) host.liberate();
    }

}

