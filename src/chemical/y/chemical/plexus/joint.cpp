
#include "y/chemical/plexus/joint.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Joint:: ~Joint() noexcept {}

        Joint:: Joint(const Cluster &cluster) noexcept :
        mine(cluster),
        neqs(mine.size),
        nspc(mine.species.size),
        key_(mine)
        {
        }
        
        const Joint::KeyType & Joint:: key() const noexcept
        {
            return key_;
        }

    }

}

