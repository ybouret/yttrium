
#include "y/memory/strap.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/memory/pages.hpp"
#include "y/calculus/align.hpp"
#include "y/random/shuffle.hpp"
#include "y/memory/out-of-reach.hpp"

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
    void fill(ListOf<block> &blocks,
              Memory::Strap *strap,
              Random::Bits  &ran)
    {
        while(true)
        {
            size_t blockSize = ran.leq(200);
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

    Random::Rand   ran;

    void          *wksp[ Memory::Page::DefaultBytes / sizeof(void*) ];
    Memory::Strap *strap = new (wksp) Memory::Strap(wksp,sizeof(wksp));
    strap->displayInfo(0);



    CxxListOf<block> blocks;
    fill(blocks,strap,ran);


    for(size_t iter=0;iter<16;++iter)
    {
        Random::Shuffle::List(blocks,ran);
        const size_t half = blocks.size/2;
        while(blocks.size>half)
        {
            block *b = blocks.popTail();
            Memory::Strap::Release(b->addr);
            delete b;
        }

        fill(blocks,strap,ran);
    }

    while(blocks.size)
    {
        block *b = blocks.popTail();
        Memory::Strap::Release(b->addr);
        delete b;
    }


    std::cerr << "bytes = " << strap->myShift() << " / " << iLog2<sizeof(wksp)>::Value << std::endl;




}
Y_UDONE()

