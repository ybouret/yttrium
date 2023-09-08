#include "y/memory/strap.hpp"
#include "y/check/static.hpp"
#include "y/calculus/align.hpp"
#include "y/calculus/base2.hpp"
#include "y/system/exception.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

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

        const size_t       Strap:: MinBytes = MinBlockCount * sizeof(Block);
        const char * const Strap:: CallSign = "Memory::Strap" ;

        Strap:: Strap(void *addr, const size_t size) noexcept :
        next(0),
        prev(0),
        head(0),
        tail(0)
        {
            Y_STATIC_CHECK(sizeof(Strap)==sizeof(Block),MismatchSizes);
            assert(size>=MinBlockCount*sizeof(Block));
            assert(IsPowerOfTwo(size));
            
            //------------------------------------------------------------------
            // available blocks after header = this
            //------------------------------------------------------------------
            const size_t numBlocks = (size - sizeof(Block)) / sizeof(Block); assert(numBlocks>=MinBlockCount-1);
            head                   = static_cast<Block *>(addr)+1;
            tail                   = &head[numBlocks-1];

            //------------------------------------------------------------------
            // prepare head
            //------------------------------------------------------------------
            head->next = tail;   // linking
            head->prev = 0;      // sentinel
            head->used = 0;      // means free
            head->size = (numBlocks-2) * sizeof(Block); // initial full capacity

            //------------------------------------------------------------------
            // prepare tail
            //------------------------------------------------------------------
            tail->prev = head; // linking
            tail->next = 0 ;   // sentinel
            tail->used = this; // means always used
            tail->size = 0;    // and no length
        }

        Strap:: ~Strap() noexcept
        {
            assert(head);
            assert(tail);
            bool   leaking = false;
            size_t count   = 0;
            for(const Block *block=head;block!=tail;block=block->next)
            {
                if(block->used)
                {
                    ++count;
                    if(!leaking)
                    {
                        std::cerr << "*** [" << CallSign << "] leaking |";
                        leaking = true;
                    }
                    std::cerr << block->size << "|";
                }
            }
            if(leaking)
            {
                std::cerr << "#=" << count << std::endl;
            }
        }

        bool Strap:: isEmpty() const noexcept
        {
            assert(0!=head);
            assert(0!=tail);
            assert(0!=tail->used);
            return (0 == head->used) && (tail == head->next);
        }


        unsigned Strap:: myShift() const noexcept
        {
            assert(0!=head);
            assert(0!=tail);
            assert(tail>head);
            const size_t blocks = static_cast<size_t>(tail-head)+2; assert(blocks>=MinBlockCount);
            unsigned     shift  = MinBlockShift; 
            size_t       count  = MinBlockCount; assert(count<=blocks);
            while(count<blocks)
            {
                ++shift;
                count <<= 1;
            }
            return shift + iLog2Of<Block>::Value;
        }



        size_t Strap:: BlockSizeFor(const size_t blockSize) noexcept
        {
            return blockSize <= sizeof(Block) ? sizeof(Block) : Y_ALIGN_TO(Block,blockSize);
        }

        unsigned Strap:: ShiftToHold(const size_t blockSize)
        {
            size_t bs = BlockSizeFor(blockSize);
            bs += 3*sizeof(Block); // Strap+Head+Tail
            if(bs>Base2<size_t>::MaxPowerOfTwo) throw Specific::Exception(CallSign,"blockSize exceeds capacity");
           //return Base2<size_t>::Log( NextPowerOfTwo(bs) );
            return Base2<size_t>::LogFor(bs);
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
                assert(0!=curr);
                if(0!=curr->used || curr->size<bs) continue;
                best = curr;
                goto FOUND;
            }
            return 0; // no free, matching block
            
        FOUND:
            //------------------------------------------------------------------
            // look for a better block
            //------------------------------------------------------------------
            assert(0!=best);
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

            //------------------------------------------------------------------
            // do we need to cut after it ?
            //------------------------------------------------------------------
            static const size_t CutThreshold = 2 * sizeof(Block);
            const size_t        optBlockSize = BlockSizeFor(bs); assert(optBlockSize>0); assert( 0 == (optBlockSize%sizeof(Block)) );
            const size_t        optEmptySize = blen - optBlockSize;

            if(optEmptySize>=CutThreshold)
            {
                const size_t optBlocks = optBlockSize / sizeof(Block); assert(optBlocks>0);
                Block *gate = &best[1+optBlocks];
                Block *next = best->next;         assert(0!=next);
                gate->next  = next;
                gate->prev  = best;
                best->next  = gate;
                next->prev  = gate;
                gate->used  = 0;
                UpdateSizeOf(gate);
                best->size  = optBlockSize;
                assert(best->size+sizeof(Block)+gate->size==blen);
            }

            //------------------------------------------------------------------
            // update status
            //------------------------------------------------------------------
            blockSize  = best->size;
            best->used = this;
            return memset(best+1,0,blockSize);
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

        Strap *Strap:: Release(void *blockAddr) noexcept
        {
            //------------------------------------------------------------------
            // local definitions
            //------------------------------------------------------------------
            static const unsigned Solo  = 0x00;
            static const unsigned Next  = 0x01;
            static const unsigned Prev  = 0x02;
            static const unsigned Both  = Next|Prev;

            //------------------------------------------------------------------
            // get parent block, with sanity check
            //------------------------------------------------------------------
            assert(0!=blockAddr);
            Block *block = static_cast<Block *>(blockAddr)-1; // parent address
            assert(0!=block->used);                           // strap address
            assert(block->size>0);                            // current size
            assert( 0 == (block->size%sizeof(Block)) );       // sanity check
            assert( 0 != block->next);                        // tail is never used

            //------------------------------------------------------------------
            // get Strap and compute fusion flags
            //------------------------------------------------------------------
            Strap   *strap = block->used;
            unsigned flags = Solo;
            Block   *prev  = block->prev; if(prev && !(prev->used)) flags |= Prev;
            Block   *next  = block->next; if(!(next->used))         flags |= Next;


            switch(flags)
            {

                case Solo:
                    //----------------------------------------------------------
                    // no fusion
                    //----------------------------------------------------------
                    block->used = 0;
                    break;

                case Prev:
                    //----------------------------------------------------------
                    // fusion with previous block
                    //----------------------------------------------------------
                    prev->next = next;
                    next->prev = prev;
                    UpdateSizeOf(prev);
                    break;

                case Next: {
                    //----------------------------------------------------------
                    // fusion with next block
                    //----------------------------------------------------------
                    assert(next!=strap->tail);
                    assert(0!=next->next);
                    Block *after = next->next;
                    block->next = after;
                    after->prev = block;
                    block->used = 0;
                    UpdateSizeOf(block);
                } break;


                case Both: {
                    //----------------------------------------------------------
                    // fusion with both prev and next block
                    //----------------------------------------------------------
                    assert(next!=strap->tail);
                    assert(0!=next->next);
                    Block *after = next->next;
                    prev->next   = after;
                    after->prev  = prev;
                    UpdateSizeOf(prev);
                }

            }


            return strap;
        }


        bool Strap:: owns(const void *blockAddr) const noexcept
        {
            assert(0!=blockAddr);
            const Block * const guess = static_cast<const Block *>(blockAddr)-1;
            for(const Block *block=head;block!=tail;block=block->next)
            {
                if(guess==block) return true;
            }
            return false;
        }


        size_t Strap:: blockSizeOf(const void *blockAddr) const noexcept
        {
            assert(0!=blockAddr);
            assert(owns(blockAddr));
            const Block * const block = static_cast<const Block *>(blockAddr)-1;
            return block->size;
        }


    }

}

