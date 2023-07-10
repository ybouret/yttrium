
#include "y/memory/straps.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/shuffle.hpp"
#include "y/memory/out-of-reach.hpp"

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

    Random::Rand   ran;
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
                blk.size = ran.leq(100);
                blk.addr = straps.acquire(blk.size);
            }

            for(size_t iter=0;iter<16;++iter)
            {
                Random::Shuffle::Tableau(blocks,NB,ran);
                for(size_t i=NB-1;i>=NB/2;--i)
                {
                    block_t &blk = blocks[i];
                    straps.release(blk.addr);
                }

                for(size_t i=NB/2;i<NB;++i)
                {
                    block_t &blk = blocks[i];
                    blk.size = ran.leq(100);
                    blk.addr = straps.acquire(blk.size);
                }
            }


            Random::Shuffle::Tableau(blocks,NB,ran);
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

