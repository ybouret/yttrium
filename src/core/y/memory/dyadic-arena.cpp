
#include "y/memory/dyadic-arena.hpp"


namespace Yttrium
{

    namespace Memory
    {
        DyadicArena:: ~DyadicArena() noexcept
        {
        }

        DyadicArena:: DyadicArena(const unsigned userShift,
                                  Album         &userDataPages,
                                  const size_t   userPageBytes) :
        Dyad(userShift),
        Arena(bytes,userDataPages,userPageBytes)
        {

        }


        void * DyadicArena :: acquire()
        {
            return acquireBlock();
        }

        void DyadicArena:: release(void *addr) noexcept
        {
            assert(0!=addr);
            releaseBlock(addr);
        }
    }

}
