
#include "y/memory/quanta.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/straps.hpp"

namespace Yttrium
{

    namespace Memory
    {
        Quanta:: Quanta(Blocks &userBlocks,
                        Straps &userStraps) noexcept :
        blocks(userBlocks),
        straps(userStraps)
        {
            
        }

        Quanta:: ~Quanta() noexcept
        {

        }

        void *Quanta:: acquire(size_t blockSize)
        {
            return blockSize <= LimitSize ? blocks.acquire(blockSize) : straps.acquire(blockSize);
        }

        void Quanta:: release(void *blockAddr, const size_t blockSize) noexcept
        {
            assert(0!=blockAddr);
            assert(blockSize>0);
            if(blockSize<=LimitSize)
            {
                blocks.release(blockAddr,blockSize);
            }
            else
            {
                straps.release(blockAddr);
            }
        }


    }

}

