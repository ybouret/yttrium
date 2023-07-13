

#include "y/memory/allocator/global.hpp"
#include "y/memory/legacy.hpp"

namespace Yttrium
{

    namespace Memory
    {
        const char * const      Global:: CallSign = "Memory::Global";



        static void   *workspace[ Y_WORDS_FOR(Legacy) ];
        static Legacy *legacy = 0;

        Global:: ~Global() noexcept
        {
            assert(0!=legacy);
            OutOfReach::Naught(legacy);
            legacy = 0;
        }

        Global:: Global() :
        Singleton<Global,Locking::Nucleus>(),
        Allocator()
        {
            legacy = new ( Y_STATIC_ZARR(workspace) ) Legacy();
        }

        const char * Global:: variety() const noexcept
        {
            return CallSign;
        }

        void * Global:: acquire(size_t & count, const size_t blockSize)
        {
            assert(0!=legacy);
            const size_t request = count * blockSize;
            try {
                void *p = legacy->acquire(request); // system locked
                count = request;
                assert( OutOfReach::Are0(p,count) );
                return p;
            }
            catch(...)
            {
                count = 0;
                throw;
            }
        }


        void Global:: release(void *&entry, size_t &count) noexcept
        {
            assert(0!=legacy);
            legacy->release(entry,count); // system locked
            entry = 0;
            count = 0;
        }

        

    }

}

