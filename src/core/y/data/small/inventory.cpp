
#include "y/data/small/inventory.hpp"
#include "y/object.hpp"

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
                Object::operator delete( pool.query(), blockSize );
        }

        void * Inventory:: zquery() noexcept
        {
            assert(pool.size>0);
            return memset( pool.query(), 0, blockSize );
        }

        void Inventory:: zstore(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            pool.store( static_cast<Node*>( memset(blockAddr,0,sizeof(Node))) );
        }


        size_t Inventory:: inside() const noexcept
        {
            return pool.size;
        }

    }

}
