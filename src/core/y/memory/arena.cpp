
#include "y/memory/arena.hpp"
#include "y/memory/pages.hpp"
#include "y/memory/album.hpp"
#include "y/calculus/align.hpp"
#include "y/type/utils.hpp"
#include <iomanip>

namespace Yttrium
{

    namespace Memory
    {

        const char * const Arena :: CallSign = "Memory::Arena";
        
        Arena:: ~Arena() noexcept
        {


            while(size>0)
            {
                Chunk         *chunk = popTail();
                const unsigned inUse = chunk->operatedNumber;
                dataPages.store( chunk );
                if(inUse>0)
                {
                    std::cerr << "*** Memory::Chunk[" << std::setw(4) << blockSize << "] missing #" << inUse << std::endl;
                }
            }

            acquiring = 0;
            releasing = 0;
            available = 0;
            
        }

        Chunk * Arena:: queryChunk()
        {
            Chunk *chunk = Chunk::Create(blockSize, dataPages.query(), dataPages.bytes);
            assert(0!=chunk);
            assert(numBlocks==static_cast<size_t>(chunk->providedNumber));
            return chunk;
        }


        Arena:: Arena(const size_t  userBlockSize,
                      Album        &userDataPages,
                      const size_t  userPageBytes) :
        ListOf<Chunk>(),
        acquiring(0),
        releasing(0),
        wandering(0),
        available(0),
        blockSize(userBlockSize),
        numBlocks(0),
        dataPages( userDataPages[ComputeShift(blockSize,userPageBytes,Coerce(numBlocks))] ),
        addBlocks(numBlocks-1)
        {
            std::cerr << "Arena: blockSize=" << std::setw(4) << blockSize << " pageBytes: " << std::setw(6) << userPageBytes << " -> " << std::setw(6) << dataPages.bytes << " @" << numBlocks << " bpp" << std::endl;
            acquiring = releasing = wandering = pushTail( queryChunk()  );
            available = numBlocks;
        }

        static const size_t Header = sizeof(Chunk);

        static inline uint8_t getNumBlocks(const size_t blockSize,
                                           const size_t dataBytes) noexcept
        {
            assert(blockSize>0);
            assert(dataBytes>Header);
            const size_t nb = (dataBytes - Header) / blockSize;
            if(nb<=Arena::MaxBlocksPerChunk)
                return static_cast<uint8_t>(nb);
            else
                return Arena::MaxBlocksPerChunk;
        }

        unsigned Arena:: ComputeShift(const size_t blockSize,
                                      const size_t pageBytes,
                                      size_t      &numBlocks) noexcept
        {

            assert(blockSize>0);

            //------------------------------------------------------------------
            //
            // compute the smallest required dataBytes for a page
            //
            //------------------------------------------------------------------
            size_t chunkSize = MinBlocksPerChunk * blockSize;
            size_t dataBytes = NextPowerOfTwo(Header + Y_MEMALIGN(chunkSize));
            numBlocks        = getNumBlocks(blockSize,dataBytes); assert(numBlocks>=MinBlocksPerChunk);


            if(pageBytes<=dataBytes)
            {
                //--------------------------------------------------------------
                // get the smallest dataBytes after minimal value
                //--------------------------------------------------------------
                const unsigned shift = Max( Base2<size_t>::Log(dataBytes), Pages::MinShift );
                dataBytes            = Base2<size_t>::One << shift;
                numBlocks            = getNumBlocks(blockSize,dataBytes);
                return shift;
            }
            else
            {
                //--------------------------------------------------------------
                // start from the highest fitted dataBytes
                //--------------------------------------------------------------
                chunkSize      = MaxBlocksPerChunk * blockSize;
                dataBytes      = Header + Y_MEMALIGN(chunkSize);
                unsigned shift = Base2<size_t>::Log(dataBytes);
                dataBytes      = Base2<size_t>::One << shift;    assert(getNumBlocks(blockSize,dataBytes)>=MinBlocksPerChunk);

                //--------------------------------------------------------------
                // and decrease until fitting the page bytes
                //--------------------------------------------------------------
                while(dataBytes>pageBytes)
                {
                    --shift;
                    dataBytes >>= 1;
                }
                numBlocks = getNumBlocks(blockSize,dataBytes);
                return shift;
            }

        }

    }

}

#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Memory
    {

        void *Arena:: acquire()
        {
            assert(0!=acquiring);
            assert(0!=releasing);

            if(acquiring->stillAvailable>0)
            {
                //--------------------------------------------------------------
                //
                // fully cached!
                //
                //--------------------------------------------------------------
                --available;
                if(wandering==acquiring)
                {
                    assert(0==wandering->operatedNumber);
                    wandering = 0;
                }
                return acquiring->acquire(blockSize);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // look up
                //
                //--------------------------------------------------------------
                if(available)
                {
                    //----------------------------------------------------------
                    // some memory somewhere: start interleaved lookup
                    //----------------------------------------------------------
                    Chunk *lower = acquiring->prev;
                    Chunk *upper = acquiring->next;
                    while(0!=lower && 0!=upper)
                    {
                        if(lower->stillAvailable)
                        {
                            acquiring = lower;
                            goto FOUND;
                        }

                        if(upper->stillAvailable)
                        {
                            acquiring = upper;
                            goto FOUND;
                        }
                        lower=lower->prev;
                        upper=upper->next;
                    }

                    assert(0==lower || 0==upper);

                    while(0!=lower)
                    {
                        if(lower->stillAvailable)
                        {
                            acquiring = lower;
                            goto FOUND;
                        }
                        lower=lower->prev;
                    }

                    while(0!=upper)
                    {
                        if(upper->stillAvailable)
                        {
                            acquiring = upper;
                            goto FOUND;
                        }
                        upper=upper->next;
                    }

                    throw Specific::Exception(CallSign,"corrupted acquire()");

                FOUND:
                    assert(acquiring->stillAvailable);
                    --available;
                    if(wandering==acquiring)
                    {
                        assert(0==wandering->operatedNumber);
                        wandering = 0;
                    }
                    return acquiring->acquire(blockSize);
                }
                else
                {
                    //----------------------------------------------------------
                    // get a fresh chunk
                    //----------------------------------------------------------
                    acquiring = insertByIncreasingAddress( queryChunk() );
                    assert(acquiring->providedNumber==numBlocks);
                    assert(ListOps::CheckIncreasingAddresses(*this));
                    available += addBlocks;
                    return acquiring->acquire(blockSize);
                }
            }
        }

    }
}


#include "y/system/error.hpp"

namespace Yttrium
{

    namespace Memory
    {

        void Arena:: release(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            assert(0!=acquiring);
            assert(0!=releasing);


            switch( releasing->whose(blockAddr) )
            {
                case Chunk::IsOwnedByThis:
                    goto FOUND;

                case Chunk::IsOwnedByNext:
                    for(Chunk *chunk=releasing->next;chunk;chunk=chunk->next)
                    {
                        if(chunk->entails(blockAddr))
                        {
                            releasing = chunk;
                            goto FOUND;
                        }
                    }
                    break;

                case Chunk::IsOwnedByPrev:
                    for(Chunk *chunk=releasing->prev;chunk;chunk=chunk->prev)
                    {
                        if(chunk->entails(blockAddr))
                        {
                            releasing = chunk;
                            goto FOUND;
                        }
                    }
                    break;
            }

            Libc::CriticalError(EINVAL, "%s unregistered address %p", CallSign, blockAddr);


        FOUND:
            if(releasing->release(blockAddr,blockSize))
            {
                if(wandering)
                {
                    assert(wandering!=releasing);
                    Chunk *toRelease = Min(releasing,wandering);
                    dataPages.store( pop(toRelease) );
                }
            }


        }
    }

}
