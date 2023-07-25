#include "y/data/small/inventory.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Small
    {
        Inventory:: Inventory(const size_t bs) noexcept :
        blockSize(bs),
        pool()
        {
            assert(blockSize>=sizeof(Node));
        }

        Inventory:: ~Inventory() noexcept
        {
            release();
        }

        void Inventory:: release() noexcept
        {
            while(pool.size>0)
                zrelease( pool.query(), blockSize );
        }


        void * Inventory:: getFlat()
        {
            return (pool.size>0) ? memset( pool.query(), 0, blockSize ) : zacquire(blockSize);
        }

        void Inventory:: putFlat(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            pool.store( static_cast<Node*>( memset(blockAddr,0,sizeof(Node))) );
        }


        size_t Inventory:: stowage() const noexcept
        {
            return pool.size;
        }

    }

}
