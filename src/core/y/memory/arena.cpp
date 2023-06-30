
#include "y/memory/arena.hpp"
#include "y/memory/pages.hpp"
#include "y/calculus/align.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Memory
    {

        Arena:: ~Arena() noexcept
        {
        }

        Arena:: Arena(const size_t  userBlockSize,
                      Pages        &userDataPages) :
        ListOf<Chunk>(),
        blockSize(userBlockSize),
        dataPages(userDataPages)
        {

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
                                      uint8_t     &numBlocks) noexcept
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
