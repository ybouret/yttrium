
#include "y/memory/object/sentries.hpp"

namespace Yttrium
{
    namespace Memory
    {

        ObjectSentries:: ObjectSentries(const void *blockAddr) :
        ObjectLocator(blockAddr),
        prev(*this, & ObjectLocator:: prevAddr),
        next(*this, & ObjectLocator:: nextAddr)
        {

        }

        ObjectSentries:: ~ObjectSentries() noexcept
        {

        }

    }

}
