
#include "y/memory/zcache.hpp"
#include "y/calculus/align.hpp"
#include "y/concurrent/memory.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Memory
    {
        static inline size_t BlockSizeFor(const size_t bs)
        {
            if(bs<=sizeof(ZombieNode))
            {
                return sizeof(ZombieNode);
            }
            else
            {
                return Y_MEMALIGN(bs);
            }
        }

        static Arena & GetArena(const size_t blockSize,
                                Lockable    &giantLock)
        {
            Y_LOCK(giantLock);
            return Concurrent::Mem::BlocksInstance()[blockSize];
        }

        ZombieCache:: ZombieCache(const size_t bs) :
        ZombiePool(),
        blockSize( BlockSizeFor(bs)  ),
        giantLock( Lockable::Giant() ),
        coreArena( GetArena(blockSize,giantLock) )
        {

        }

        void ZombieCache:: empty() noexcept
        {
            Y_LOCK(giantLock);
            while(size>0)
            {
                coreArena.releaseBlock( query() );
            }
        }

        void ZombieCache:: release() noexcept
        {
            empty();
        }

        ZombieCache:: ~ZombieCache() noexcept
        {
            empty();
        }

        void   ZombieCache:: reserve(size_t n)
        {
            Y_LOCK(giantLock);
            while(n-- > 0) store( static_cast<ZombieNode *>( coreArena.acquireBlock()) );
        }


        void * ZombieCache:: acquireBlock()
        {
            if(size>0)
                return OutOfReach::Zero( query(), blockSize);
            else
            {
                Y_LOCK(giantLock);
                return coreArena.acquireBlock();
            }
        }

        void ZombieCache:: release(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            store( static_cast<ZombieNode *>( OutOfReach::Zero(blockAddr,sizeof(ZombieNode))) );
        }

        


    }
}

