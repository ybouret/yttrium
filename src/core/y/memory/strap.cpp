#include "y/memory/strap.hpp"
#include "y/check/static.hpp"
#include "y/calculus/align.hpp"

#include <iostream>
#include <iomanip>

namespace Yttrium
{

    namespace Memory
    {

        typedef UnsignedInt<sizeof(void*)>::Type len_t;

        struct Block
        {
            Block *next;
            Block *prev;
            Strap *used;
            len_t  size;
        };

        static inline void UpdateSizeOf(Block *self) noexcept
        {
            assert(0!=self);
            assert(0!=self->next);
            assert(self->next>self);
            self->size = static_cast<size_t>(self->next - (self+1)) * sizeof(Block);
        }



        Strap:: Strap(void *addr, const size_t size) noexcept :
        next(0),
        prev(0),
        head(0),
        tail(0)
        {
            Y_STATIC_CHECK(sizeof(Strap)==sizeof(Block),MismatchSizes);
            assert(size>=4*sizeof(Block));
            
            const size_t numBlocks = (size - sizeof(Block)) / sizeof(Block); assert(numBlocks>=3);
            head = static_cast<Block *>(addr)+1;
            tail = &head[numBlocks-1];

            head->next = tail;   // linking
            head->prev = 0;      // sentinel
            head->used = 0;      // means free
            head->size = (numBlocks-2) * sizeof(Block); // initial full capacity

            tail->prev = head; // linking
            tail->next = 0 ;   // sentinel
            tail->used = this; // means always used
            tail->size = 0;    // and no length
        }

        Strap:: ~Strap() noexcept
        {
        }

        size_t Strap:: BlockSizeFor(const size_t blockSize) noexcept
        {
            return blockSize <= sizeof(Block) ? sizeof(Block) : Y_ALIGN_TO(Block,blockSize);
        }


        void * Strap:: acquire(size_t &blockSize) noexcept
        {
            Block       *best = 0;
            const size_t bs   = blockSize;
            //------------------------------------------------------------------
            // look for first ready block
            //------------------------------------------------------------------
            for(Block *curr=head;curr;curr=curr->next)
            {
                if(curr->used || curr->size<bs) continue;
                best = curr;
                goto FOUND;
            }
            return 0;
            
        FOUND:
            // initialize first length
            size_t blen  = best->size;
            for(Block *curr=best->next;curr;curr=curr->next)
            {
                const size_t temp = curr->size;
                if(curr->used || temp<bs || temp>=blen) continue;
                assert(0==curr->used);
                assert(temp<blen);
                best = curr;
                blen = temp;
            }

            static const size_t CutThreshold = 2 * sizeof(Block);
            const size_t        optBlockSize = BlockSizeFor(bs); assert(optBlockSize>0); assert( 0 == (optBlockSize%sizeof(Block)) );
            const size_t        optEmptySize = blen - optBlockSize;
            //std::cerr << "bs=" << bs << " -> " << optBlockSize << " in " << blen << " => empty=" << optEmptySize << "/cut=" << CutThreshold << std::endl;

            if( optEmptySize>=CutThreshold)
            {
                //std::cerr << "=> Cut!" << std::endl;
                const size_t optBlocks = optBlockSize / sizeof(Block); assert(optBlocks>0);
                Block *gate = &best[1+optBlocks];
                Block *next = best->next;
                gate->next  = next;
                gate->prev  = best;
                best->next  = gate;
                next->prev  = gate;
                gate->used  = 0;
                UpdateSizeOf(gate);
                //gate->size  = (gate->next - (gate+1)) * sizeof(Block);
                best->size  = optBlockSize;
                assert(best->size+sizeof(Block)+gate->size==blen);
            }

            blockSize  = best->size;
            best->used = this;
            return best+1;
        }

        void Strap:: displayInfo(size_t indent) const
        {
            Core::Indent(std::cerr,indent) << '|';
            for(const Block *block=head;block!=tail;block=block->next)
            {
                std::cerr << '[';
                if(block->used) std::cerr << '*'; else std::cerr << '.';
                std::cerr << block->size;
                std::cerr << ']';
            }
            std::cerr << '#';
            std::cerr << '|' << std::endl;
        }

        void Strap:: Release(void *blockAddr) noexcept
        {
            static const unsigned Solo  = 0x00;
            static const unsigned Next  = 0x01;
            static const unsigned Prev  = 0x02;
            static const unsigned Both  = Next|Prev;

            assert(0!=blockAddr);
            Block *block = static_cast<Block *>(blockAddr)-1;
            assert(0!=block->used);
            assert(0!=block->prev || 0!=block->next);
            assert(block->size>0);
            assert( 0 == (block->size%sizeof(Block)) );
            assert( 0 != block->next);

            //std::cerr << "-- releasing " << block->size << std::endl;
            Strap *strap = block->used;

            unsigned flag  = Solo;
            Block   *prev  = block->prev; if(prev && !(prev->used)) flag |= Prev;
            Block   *next  = block->next; if(!(next->used))         flag |= Next;


            switch(flag)
            {
                case Solo:
                    block->used = 0;
                    break;

                case Prev:
                    prev->next = next;
                    next->prev = prev;
                    UpdateSizeOf(prev);
                    break;

                case Next: {
                    assert(next!=strap->tail);
                    assert(0!=next->next);
                    Block *after = next->next;
                    block->next = after;
                    after->prev = block;
                    block->used = 0;
                    UpdateSizeOf(block);
                } break;


                case Both: {
                    assert(next!=strap->tail);
                    assert(0!=next->next);
                    Block *after = next->next;
                    prev->next   = after;
                    after->prev  = prev;
                    UpdateSizeOf(prev);
                }

            }


            strap->displayInfo(0);
        }


    }

}

