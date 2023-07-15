
#include "y/apex/m/block.hpp"
#include "y/apex/m/archon.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Nexus
        {
            void Block:: TooBigException(const unsigned int usrShift, const unsigned int maxShift)
            {
                assert(usrShift>maxShift);
                throw Specific::Exception("Apex::Memory",
                                          "Block 2^%u > 2^%u",usrShift,maxShift);
            }

            void * Block:: Acquire(unsigned &shift)
            {
                static Archon &archon = Archon::Instance();
                return archon.acquire(shift);
            }

            void  Block:: Release(void *entry, const unsigned shift) noexcept
            {
                static Archon &archon = Archon::Location();
                archon.release(entry,shift);
            }

        }
    }

}

