//! \file


#ifndef Y_Memory_ZCache_Included
#define Y_Memory_ZCache_Included 1

#include "y/lockable.hpp"
#include "y/data/pool.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{
    namespace Memory
    {

        class Arena;

        struct ZombieNode
        {
            ZombieNode *next;
            ZombieNode *prev;
        };

        typedef PoolOf<ZombieNode> ZombiePool;

        class ZombieCache : public ZombiePool, public Releasable
        {
        protected:
            explicit ZombieCache(const size_t bs);

        public:
            virtual ~ZombieCache() noexcept;

            void        reserve(size_t n);
        protected:
            void       *acquireBlock();

        public:
            virtual void release() noexcept;
            void         release(void *blockAddr) noexcept;

            const size_t blockSize;
            Lockable    &giantLock;
            Arena       &coreArena;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZombieCache);
            void empty() noexcept;
        };


        
    }

    template <typename T>
    class ZombieCache : public Memory::ZombieCache
    {
    public:
        inline explicit ZombieCache() : Memory::ZombieCache(sizeof(T)) {}
        inline virtual ~ZombieCache() noexcept {}

        inline T *  acquire() { return static_cast<T*>(acquireBlock()); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ZombieCache);
    };
    
}

#endif

