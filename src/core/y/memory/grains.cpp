
#include "y/memory/grains.hpp"
#include "y/concurrent/memory.hpp"
#include "y/memory/quanta.hpp"

namespace Yttrium
{

    namespace Memory
    {

        const char * const      Grains:: CallSign = "Memory::Grains";


        static Quanta *quanta = 0;

        Grains:: ~Grains() noexcept
        {
        }

        Grains:: Grains() : Singleton<Grains,Locking::Nucleus>()
        {
            quanta = & Concurrent::Mem::QuantaInstance();
        }

        void * Grains:: acquire(size_t blockSize)
        {
            assert(quanta);
            assert(blockSize>0);
            Y_LOCK(access);
            return quanta->acquire(blockSize);
        }

        void Grains:: release(void *blockAddr, const size_t blockSize) noexcept
        {
            assert(quanta);
            assert(0!=blockAddr);
            assert(blockSize>0);

            Y_LOCK(access);
            quanta->release(blockAddr,blockSize);

        }
    }

}



#include "y/memory/object/locator.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/straps.hpp"

namespace Yttrium
{

    namespace Memory
    {

        void Grains::lookFor(ObjectLocator &objloc) const
        {
            if( quanta->blocks.owns(objloc.where,Coerce(objloc.arena),Coerce(objloc.chunk)) ) return;
            if( quanta->straps.find(objloc.where,Coerce(objloc.strap)) )                      return;
        }


    }

}
