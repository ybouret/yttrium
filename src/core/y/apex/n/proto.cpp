#include "y/apex/n/proto.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Nexus
        {
            Proto:: ~Proto() noexcept {}
            Proto:: Proto() noexcept {}

            size_t Proto:: BitsToBytes(const size_t numBits) noexcept
            {
                return Y_ALIGN_ON(8,numBits) >> 3;
            }

        }

    }


}


