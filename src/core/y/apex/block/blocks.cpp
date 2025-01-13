
#include "y/apex/block/blocks.hpp"
#include "y/data/rework.hpp"
#include "y/sort/merge.hpp"

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


        void Blocks:: gc() noexcept {
            Block::List L;
            Rework::PoolToList(L,my);
            MergeSort::Call(L,ListOps::IncreasingAddresses<Block>);
            ListOps::CheckIncreasingAddresses(L);
            const size_t maxSize = 1 + (L.size>>1);
            while(L.size>maxSize) delete L.popTail();
            Rework::ListToPool(my,L);
        }

        Block * Blocks:: query() {
            Block * const block = (my.size>0) ? my.query() : new Block(shift);
            assert( shift == block->shift );
            assert( Memory::OutOfReach::Are0(block->entry, block->range) );
            return block;
        }

        void Blocks:: store(Block * const block) noexcept
        {
            assert(0!=block);
            assert(block->shift==shift);
            my.store(block)->ldz();
        }


    }

}
