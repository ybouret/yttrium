
#include "y/apex/block/blocks.hpp"
#include "y/data/rework.hpp"
#include "y/sort/merge.hpp"
#include "y/data/gc/cxx-pool.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Blocks:: Blocks(const unsigned _shift) noexcept :
        Proxy<const Block::Pool>(),
        my(),
        shift(_shift)
        {
            assert(shift>=Block::MinShift);
            assert(shift<=Block::MaxShift);
        }

        Blocks:: ~Blocks() noexcept {}


        Y_PROXY_IMPL(Blocks,my)


        void Blocks:: gc(const size_t cycles) noexcept {
            CxxPoolGC:: Cycle(my,cycles);
        }

        Block * Blocks:: query() {
            Block * const block = (my.size>0) ? my.query() : new Block(shift);
            assert( shift == block->shift );
            assert( Memory::OutOfReach::Are0(block->entry, block->range) );
            return block;
        }

        Block * Blocks:: tryQuery() noexcept
        {
            return (my.size>0) ? my.query() : 0;
        }



        void Blocks:: store(Block * const block) noexcept
        {
            assert(0!=block);
            assert(block->shift==shift);
            my.store(block)->ldz();
        }


    }

}
