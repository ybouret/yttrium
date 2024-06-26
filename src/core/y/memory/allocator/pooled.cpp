
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/straps.hpp"
#include "y/concurrent/memory.hpp"

namespace Yttrium
{

    namespace Memory
    {
        const char * const      Pooled:: CallSign = "Memory::Pooled";

        const char * Pooled:: variety() const noexcept
        {
            return CallSign;
        }

        static Straps   * straps = 0;

        Pooled:: Pooled() : Singleton<Pooled,Locking::Nucleus>(), Allocator()
        {
            straps = & Concurrent::Mem::StrapsInstance();
        }

        Pooled:: ~Pooled() noexcept
        {
        }

        void * Pooled:: acquire(size_t & count, const size_t blockSize)
        {
            Y_LOCK(access);
            size_t request = count * blockSize;
            try {
                void *p = straps->acquire(request);
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

        void Pooled:: release(void *&entry, size_t &count) noexcept
        {
            Y_LOCK(access);
            assert(0!=entry);
            straps->release(entry);
            entry = 0;
            count = 0;
        }


        void * Pooled:: acquireLegacy(const size_t blockSize)
        {
            Y_LOCK(access);
            size_t request = blockSize;
            return straps->acquire(request);
        }

        void Pooled:: releaseLegacy(void *blockAddr, const size_t) noexcept
        {
            Y_LOCK(access);
            assert(0!=blockAddr);
            straps->release(blockAddr);
        }


    }

}

