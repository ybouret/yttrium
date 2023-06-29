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


        Chunk:: Chunk(const size_t   block_size,
                      void          *chunk_data,
                      const size_t   chunk_size) noexcept :
        first_available(0),
        still_available(num_blocks(block_size,chunk_size)),
        operated_number(0),
        provided_number(still_available),
        data( static_cast<uint8_t *>(chunk_data)  ),
        last( data + block_size * provided_number ),
        next(0),
        prev(0)
        {
            assert(Good(chunk_data,chunk_size));

            const uint8_t n = provided_number;
            for(uint8_t   q = 0, *p=data;q!=n; p += block_size)
            {
                assert(owns(p,block_size));
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

        void * Chunk:: acquire(const size_t block_size) noexcept
        {
            assert(still_available>0);
            assert(still_available<=provided_number);
            assert(operated_number+still_available==provided_number);
            assert(block_size>0);

            {
                uint8_t * p = &data[first_available*block_size]; /* get address                 */
                first_available = *p;                            /* read next available address */
                --still_available;                               /* bookkeeping                 */
                ++operated_number;                               /* bookkeeping                 */
                memset(p,0,block_size);                          /* zero memory                 */
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
            assert(operated_number>0);
            assert(operated_number+still_available==provided_number);



            //------------------------------------------------------------------
            //
            // find block and update status
            //
            //------------------------------------------------------------------
            {
                uint8_t     *to_release = static_cast<uint8_t *>(block_addr);
                const size_t indx       = (size_t)(to_release-data)/block_size;
                *to_release             = first_available;
                first_available         = (uint8_t)indx;
                ++still_available;
                return (--operated_number) <= 0;
            }
        }


#if 0

        


        
        
#include <stdio.h>
#include <string.h>
#include "y/memory/out-of-reach.h"
        
        void Y_MemoryChunk_Quit(Y_MemoryChunk *chunk,
                                const size_t   block_size)
        {
            assert(NULL!=chunk);
            assert(block_size>0);
            if(chunk->operated_number) {
                fprintf(stderr,"[Yttrium::MemoryChunk] still %2lu block_size=%3lu\n", (unsigned long)(chunk->operated_number), (unsigned long)block_size);
            }
            memset(chunk,0,sizeof(Y_MemoryChunk));
        }
        
        int Y_MemoryChunk_Owns(const Y_MemoryChunk *chunk,
                               const void          *block,
                               const size_t         block_size)
        {
            const uint8_t *p = (const uint8_t *)block;
            assert(NULL!=chunk);
            assert(NULL!=block);
            assert(block_size>0);
            if(p>=chunk->data&&p<chunk->last)
            {
                const ptrdiff_t shift = Y_OutOfReach_Diff(chunk->data,p);
                return (0 == (shift%block_size) ) ? 1 : 0;
            }
            else
            {
                return 0;
            }
            
        }
        
        Y_MemoryChunkOwner Y_MemoryChunk_Whose(const Y_MemoryChunk *chunk,
                                               const void          *block)
        {
            const uint8_t *p = (const uint8_t *)block;
            assert(NULL!=chunk);
            assert(NULL!=block);
            if(p<chunk->data)
                return Y_MemoryChunkPrev;
            else
            {
                if(p>=chunk->last)
                {
                    return Y_MemoryChunkNext;
                }
                else
                {
                    return Y_MemoryChunkSelf;
                }
            }
            
        }
        
        
        void *Y_MemoryChunk_Acquire(Y_MemoryChunk *chunk,
                                    const size_t   block_size)
        {
            assert(NULL!=chunk);
            assert(chunk->still_available>0);
            assert(chunk->still_available<=chunk->provided_number);
            assert(chunk->operated_number+chunk->still_available==chunk->provided_number);
            assert(block_size>0);
            
            {
                uint8_t * p = &chunk->data[chunk->first_available*block_size]; /* get address                 */
                chunk->first_available = *p;                                   /* read next available address */
                --chunk->still_available;                                      /* bookkeeping                 */
                ++chunk->operated_number;                                      /* bookkeeping                 */
                memset(p,0,block_size);                                        /* zero memory                 */
                return p;                                                      /* done                        */
            }
        }
        
        int Y_MemoryChunk_Release(Y_MemoryChunk *chunk,
                                  void          *block_addr,
                                  const size_t   block_size)
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert(NULL!=chunk);
            assert(block_size>0);
            assert(Y_MemoryChunk_Owns(chunk,block_addr,block_size));
            assert(chunk->operated_number>0);
            assert(chunk->operated_number+chunk->still_available==chunk->provided_number);
            
            
            
            //------------------------------------------------------------------
            //
            // find block and update status
            //
            //------------------------------------------------------------------
            {
                uint8_t     *to_release = (uint8_t *)block_addr;
                const size_t indx       = (size_t)(to_release-chunk->data)/block_size;
                *to_release             = chunk->first_available;
                chunk->first_available  = (uint8_t)indx;
                ++chunk->still_available;
                return (--chunk->operated_number) <= 0 ? 1 : 0;
            }
        }
#endif

    }
}
