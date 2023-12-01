
#include "y/concurrent/split/trek.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        Trek:: ~Trek() noexcept {}

        Trek:: Trek(const Kind k) noexcept : kind(k) {}
        Trek:: Trek(const Trek &trek) noexcept : kind(trek.kind) {}

    }

}
