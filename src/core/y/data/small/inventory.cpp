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


        void Inventory:: release_() noexcept
        {
            while(pool.size>0)
                zrelease( pool.query(), blockSize );
        }

        Inventory:: ~Inventory() noexcept
        {
            release_();
        }

        void Inventory:: release() noexcept
        {
            release_();
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

        void Inventory:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                pool.store( static_cast<Node*>( zacquire(blockSize)) );
            }
        }

    }

}
