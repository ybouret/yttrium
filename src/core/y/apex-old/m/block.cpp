
#include "y/apex/m/block.hpp"
#include "y/apex/m/archon.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        const IncreaseSize_ IncreaseSize = {};
        const NoMemoryCopy_ NoMemoryCopy = {};
        
        namespace Nexus
        {

            static const char fn[] = "Apex::Memory";

            unsigned Block:: ShiftFor(const size_t usrBytes)
            {
                if(usrBytes<=MinBytes)
                    return MinShift;

                if(usrBytes>MaxBytes) throw Specific::Exception(fn,"Too many bytes");

                unsigned shift = MinShift;
                size_t   bytes = MinBytes;
                while(bytes<usrBytes)
                {
                    bytes <<= 1;
                    ++shift;
                }
                return shift;

            }

            unsigned   Block:: ShiftInc(unsigned shift)
            {
                assert(shift>=MinShift);
                if(shift>=MaxShift)
                    throw Specific::Exception(fn,"already at maximum bytes");
                return ++shift;
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

