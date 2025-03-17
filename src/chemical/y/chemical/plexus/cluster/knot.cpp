
#include "y/chemical/plexus/cluster/knot.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ClusterKnot:: ClusterKnot(const ClusterContent::MutablePointer  &ptr) noexcept :
        ClusterContent::MutablePointer(ptr),
        next(0),
        prev(0)
        {
        }

        ClusterKnot:: ~ClusterKnot() noexcept {}
    }

}


