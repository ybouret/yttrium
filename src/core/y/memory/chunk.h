/*! \file */

#ifndef Y_Memory_Chunk_Included
#define Y_Memory_Chunk_Included 1

#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    typedef struct Y_MemoryChunk_ {
        uint8_t                 first_available; /*!< bookkeeping                                       */
        uint8_t                 still_available; /*!< bookkeeping                                       */
        uint8_t                 operated_number; /*!< bookkeeping                                       */
        uint8_t                 provided_number; /*!< initial count : still_available + operated_number */
        uint8_t                *data;            /*!< first item                                        */
        const uint8_t *         last;            /*!< first invalid item                                */
        struct Y_MemoryChunk_  *next;
        struct Y_MemoryChunk_  *prev;
    } Y_MemoryChunk;

#define Y_MemoryChunk_Static_Init  { 0,0,0,0,NULL,NULL,NULL,NULL }

    void Y_MemoryChunk_Init(Y_MemoryChunk *chunk,
                            const size_t   block_size,
                            void          *chunk_data,
                            const size_t   chunk_size);

    void Y_MemoryChunk_Quit(Y_MemoryChunk *chunk,
                            const size_t   block_size);


    int Y_MemoryChunk_Owns(const Y_MemoryChunk *chunk,
                           const void          *block,
                           const size_t         block_size);

    typedef enum
    {
        Y_MemoryChunkPrev,
        Y_MemoryChunkSelf,
        Y_MemoryChunkNext
    } Y_MemoryChunkOwner;

    Y_MemoryChunkOwner Y_MemoryChunk_Whose(const Y_MemoryChunk *chunk,
                                           const void          *block);

    void *Y_MemoryChunk_Acquire(Y_MemoryChunk *chunk, const size_t block_size);
    int   Y_MemoryChunk_Release(Y_MemoryChunk *chunk, void *block_addr, const size_t block_size);

#if defined(__cplusplus)
}
#endif

#endif
