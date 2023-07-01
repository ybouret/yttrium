#include "y/memory/chunk.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/calculus/align.hpp"
#include "y/calculus/base2.hpp"

#include <cstring>
#include <new>

namespace Yttrium
{
    namespace Memory
    {

        const char * const Chunk::CallSign = "Memory::Chunk";

        uint8_t Chunk:: GetNumBlocks(const size_t blockSize, const size_t chunkSize) noexcept
        {
            assert(blockSize>0);
            const size_t nb = chunkSize/blockSize;
            if(nb>0xff)
                return 0xff;
            else
                return static_cast<uint8_t>(nb);
        }


        Chunk:: Chunk(const size_t   blockSize,
                      void          *chunkData,
                      const size_t   chunkSize) noexcept :
        firstAvailable(0),
        stillAvailable( GetNumBlocks(blockSize,chunkSize) ),
        operatedNumber(0),
        providedNumber(stillAvailable),
        data( static_cast<uint8_t *>(chunkData)  ),
        last( data + blockSize * providedNumber ),
        next(0),
        prev(0)
        {
            assert(Good(chunkData,chunkSize));

            const uint8_t n = providedNumber;
            for(uint8_t   q = 0, *p=data;q!=n; p += blockSize)
            {
                assert(owns(p,blockSize));
                *p = ++q;
            }
        }

        bool Chunk:: owns(const void          *block,
                          const size_t         block_size) const noexcept
        {
            assert(block_size>0);

            const uint8_t *p = static_cast<const uint8_t *>(block);
            if(p>=data&&p<last)
            {
                const ptrdiff_t shift = OutOfReach:: Diff(data,p);
                return (0 == (shift%block_size) ) ? 1 : 0;
            }
            else
            {
                return 0;
            }
        }

        bool Chunk::entails(const void *blockAddr) const noexcept
        {
            const uint8_t *p = static_cast<const uint8_t *>(blockAddr);
            return (p>=data&&p<last);
        }

        Chunk::Ownership Chunk:: whose(const void *block) const noexcept
        {
            const uint8_t *p = static_cast<const uint8_t *>(block);
            if(p<data)
                return IsOwnedByPrev;
            else
            {
                if(p>=last)
                {
                    return IsOwnedByNext;
                }
                else
                {
                    return IsOwnedByThis;
                }
            }
        }

        Chunk:: ~Chunk() noexcept
        {
            assert(NULL==next);
            assert(NULL==prev);
        }

        void * Chunk:: acquire(const size_t blockSize) noexcept
        {
            assert(stillAvailable>0);
            assert(stillAvailable<=providedNumber);
            assert(operatedNumber+stillAvailable==providedNumber);
            assert(blockSize>0);

            {
                uint8_t * p = &data[firstAvailable*blockSize]; /* get address                 */
                firstAvailable = *p;                            /* read next available address */
                --stillAvailable;                               /* bookkeeping                 */
                ++operatedNumber;                               /* bookkeeping                 */
                memset(p,0,blockSize);                          /* zero memory                 */
                return p;                                        /* done                        */
            }
        }

        bool Chunk:: release(void          *block_addr,
                             const size_t   block_size) noexcept
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(block_size>0);
            assert(owns(block_addr,block_size));
            assert(operatedNumber>0);
            assert(operatedNumber+stillAvailable==providedNumber);



            //------------------------------------------------------------------
            //
            // find block and update status
            //
            //------------------------------------------------------------------
            {
                uint8_t     *toRelease = static_cast<uint8_t *>(block_addr);
                const size_t indx      = static_cast<size_t>(toRelease-data)/block_size;
                *toRelease             = firstAvailable;
                firstAvailable         = static_cast<uint8_t>(indx);
                ++stillAvailable;
                return (--operatedNumber) <= 0;
            }
        }


        static const size_t headerBytes = sizeof(Chunk);


        size_t  Chunk:: GetFlatBytes(const size_t blockSize, const uint8_t numBlocks) noexcept
        {
            assert(blockSize>0);
            const size_t dataSize = blockSize * numBlocks;
            return headerBytes + Y_MEMALIGN(dataSize);
        }

        size_t Chunk:: OptPageBytes(const size_t blockSize) noexcept
        {
            const size_t        maxPageSize = GetFlatBytes(blockSize, 255);
            const size_t        logPageSize = Base2<size_t>::Log(maxPageSize);
            return              Base2<size_t>::One << logPageSize;
        }


        uint8_t Chunk:: OptNumBlocks(const size_t blockSize, const size_t pageBytes) noexcept
        {
            assert(blockSize>0);
            if(pageBytes<=headerBytes)
                return 0;
            else
            {
                const size_t chunkSize = pageBytes - headerBytes;
                return GetNumBlocks(blockSize,chunkSize);
            }
        }

        Chunk * Chunk::Create(const size_t blockSize,
                              void        *page,
                              const size_t size) noexcept
        {
            assert(0!=page);
            assert(size>=headerBytes);

            void        *chunkData = OutOfReach::Haul(page, headerBytes);
            const size_t chunkSize = size-headerBytes;

            return new (page) Chunk(blockSize,chunkData,chunkSize);
        }

    }
}
