
#include "y/memory/straps.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"
#include "y/data/list/cxx.hpp"

using namespace Yttrium;

namespace
{
    struct block_t
    {
        void  *addr;
        size_t size;
    };

}

Y_UTEST(memory_straps)
{
    alea_seed();

    Memory::Album  album;
    {
        Memory::Straps straps(album);

        {
            size_t  bs = 0;
            void   *p  = straps.acquire(bs);
            std::cerr << "got bs=" << bs << " @" << p << std::endl;
            straps.release(p);
        }

        if(true)
        {
            static const size_t NB = 1024;
            block_t             blocks[NB];
            Y_STATIC_ZARR(blocks);

            for(size_t i=0;i<NB;++i)
            {
                block_t &blk = blocks[i];
                blk.size = alea_leq(100);
                blk.addr = straps.acquire(blk.size);
            }

            for(size_t iter=0;iter<16;++iter)
            {
                alea_shuffle(blocks,NB);
                for(size_t i=NB-1;i>=NB/2;--i)
                {
                    block_t &blk = blocks[i];
                    straps.release(blk.addr);
                }

                for(size_t i=NB/2;i<NB;++i)
                {
                    block_t &blk = blocks[i];
                    blk.size = alea_leq(100);
                    blk.addr = straps.acquire(blk.size);
                }
            }


            alea_shuffle(blocks,NB);
            for(size_t i=0;i<NB;++i)
            {
                block_t &blk = blocks[i];
                straps.release(blk.addr);
            }
        }
    }

    album.displayInfo(0);

}
Y_UDONE()

