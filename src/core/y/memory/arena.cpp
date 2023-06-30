
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


        void *Arena:: acquire()
        {
            assert(acquiring);
            assert(releasing);
            if(acquiring->stillAvailable>0)
            {
                //--------------------------------------------------------------
                //
                // fully cached!
                //
                //--------------------------------------------------------------
                --available;
                if(wandering==acquiring) wandering = 0;
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
                    // some memory somewhere
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



                FOUND:
                    assert(acquiring->stillAvailable);
                    --available;
                    if(wandering==acquiring) wandering = 0;
                    return acquiring->acquire(blockSize);
                }
                else
                {
                    // get a fresh chunk
                    acquiring = pushByAddr( queryChunk() );
                    assert(acquiring->providedNumber==numBlocks);
                    available += addBlocks;
                    return acquiring->acquire(blockSize);
                }
            }
        }

    }

}
