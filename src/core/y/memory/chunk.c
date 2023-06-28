#include "y/memory/chunk.h"


static inline uint8_t num_blocks(const size_t block_size, const size_t chunk_size)
{
    assert(block_size>0);
    const size_t nb = chunk_size/block_size;
    if(nb>0xff)
        return 0xff;
    else
        return (uint8_t)nb;
}



void Y_MemoryChunk_Init(Y_MemoryChunk *chunk,
                        const size_t   block_size,
                        void          *chunk_data,
                        const size_t   chunk_size)
{
    uint8_t      *p = (uint8_t *)chunk_data;
    uint8_t       q = 0;
    const uint8_t n = num_blocks(block_size,chunk_size);

    assert(Y_Good(chunk_data,chunk_size));
    assert(block_size>0);

    chunk->first_available = 0;
    chunk->still_available = n;
    chunk->operated_number = 0;
    chunk->provided_number = n;
    chunk->data            = p;
    chunk->last            = p+block_size*n;
    chunk->next            = NULL;
    chunk->prev            = NULL;

    for( ;q!=n; p += block_size)
    {
        assert(Y_MemoryChunk_Owns(chunk,p,block_size));
        *p = ++q;
    }

}


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
