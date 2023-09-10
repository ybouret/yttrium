
#include "y/memory/object/sentry.hpp"

namespace Yttrium
{
    namespace Memory
    {

        ObjectSentry:: ~ObjectSentry() noexcept {}

        ObjectSentry:: ObjectSentry(const ObjectLocator &objloc, ObjectLocator::Query query) noexcept :
        ObjectGuarded(objloc,query),
        Sentry(blockAddr,blockSize)
        {
        }

    }

}

