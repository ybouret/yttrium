#include "y/memory/chunk.hpp"
#include "y/memory/out-of-reach.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Memory
    {
        static inline uint8_t num_blocks(const size_t block_size, const size_t chunk_size)
        {
            assert(block_size>0);
            const size_t nb = chunk_size/block_size;
            if(nb>0xff)
                return 0xff;
            else
                return (uint8_t)nb;
        }


        Chunk:: Chunk(const size_t   blockSize,
                      void          *chunkData,
                      const size_t   chunkSize) noexcept :
        firstAvailable(0),
        stillAvailable(num_blocks(blockSize,chunkSize)),
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
                uint8_t     *to_release = static_cast<uint8_t *>(block_addr);
                const size_t indx       = (size_t)(to_release-data)/block_size;
                *to_release             = firstAvailable;
                firstAvailable         = (uint8_t)indx;
                ++stillAvailable;
                return (--operatedNumber) <= 0;
            }
        }

        size_t Chunk:: BytesFor(const uint8_t numBlocks, const size_t blockSize) noexcept
        {
            const size_t infoSize = sizeof(Chunk);
            const size_t dataSize = numBlocks * blockSize;

            return infoSize + dataSize;
        }


    }
}
