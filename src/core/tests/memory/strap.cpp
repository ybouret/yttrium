
#include "y/memory/strap.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"
#include "y/data/list/cxx.hpp"

using namespace Yttrium;

namespace
{
    class block
    {
    public:
        block(void *userAddr, const size_t userSize) noexcept :
        next(0),
        prev(0),
        addr(userAddr),
        size(userSize)
        {
        }

        ~block() noexcept
        {
        }

        block *next, *prev;
        void        *addr;
        const size_t size;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };

    static inline
    void fill(ListOf<block> &blocks, Memory::Strap *strap)
    {
        while(true)
        {
            size_t blockSize = alea_leq(100);
            void  *blockAddr = strap->acquire(blockSize);
            if(!blockAddr) break;

            blocks.pushTail( new block(blockAddr,blockSize) );
            strap->displayInfo(0);
            Y_ASSERT( Memory::OutOfReach::Are0(blocks.tail->addr,blocks.tail->size));
        }
    }

}



Y_UTEST(memory_strap)
{

    alea_seed();
    
    void          *wksp[256];
    Memory::Strap *strap = new (wksp) Memory::Strap(wksp,sizeof(wksp));
    strap->displayInfo(0);



    CxxListOf<block> blocks;
    fill(blocks,strap);


    for(size_t iter=0;iter<16;++iter)
    {
        alea_shuffle(blocks);
        const size_t half = blocks.size/2;
        while(blocks.size>half)
        {
            block *b = blocks.popTail();
            Memory::Strap::Release(b->addr);
            delete b;
        }

        fill(blocks,strap);
    }

    while(blocks.size)
    {
        block *b = blocks.popTail();
        Memory::Strap::Release(b->addr);
        delete b;
    }


    std::cerr << "bytes = " << strap->shift__() << " / " << iLog2<sizeof(wksp)>::Value << std::endl;



}
Y_UDONE()

