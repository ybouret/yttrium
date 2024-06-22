
#include "y/apex/m/batch.hpp"
#include "y/apex/m/archon.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Nexus
        {

            void * Batch:: Acquire(unsigned int &dataShift)
            {
                static Archon &archon = Archon::Instance();
                return archon.acquire(dataShift);
            }

            void  Batch:: Release(void *entry, const unsigned shift) noexcept
            {
                static Archon &archon = Archon::Location();
                archon.release(entry,shift);
            }
        }

    }

}
