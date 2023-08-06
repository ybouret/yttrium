
#include "y/memory/blanks.hpp"
#include "y/calculus/align.hpp"
#include "y/concurrent/memory.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/rework.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Memory
    {
        static inline size_t BlockSizeFor(const size_t bs)
        {
            if(bs<=sizeof(BlankNode))
            {
                return sizeof(BlankNode);
            }
            else
            {
                return Y_MEMALIGN(bs);
            }
        }

        static Arena & GetArena(const size_t blockSize,
                                Lockable    &giantLock)
        {
            Y_LOCK(giantLock);
            return Concurrent::Mem::BlocksInstance()[blockSize];
        }

        Blanks:: Blanks(const size_t userBlockSize,
                        const size_t startCapacity) :
        BlankNode::Pool(),
        allocated(0),
        giantLock( Lockable::Giant() ),
        coreArena( GetArena(BlockSizeFor(userBlockSize),giantLock) )
        {
            try {
                reserve(startCapacity);
            }
            catch(...)
            {
                empty();
                throw;
            }
        }

        void Blanks:: empty() noexcept
        {
            Y_LOCK(giantLock);
            while(size>0)
            {
                coreArena.releaseBlock( query() );
            }
        }

        void Blanks:: release() noexcept
        {
            empty();
        }

        Blanks:: ~Blanks() noexcept
        {
            empty();
        }

        void   Blanks:: reserve(size_t n)
        {
            Y_LOCK(giantLock);
            while(n-- > 0) store( static_cast<BlankNode *>( coreArena.acquireBlock()) );
        }


        void * Blanks:: fetchBlank()
        {
            if(size>0)
            {
                ++Coerce(allocated);
                return OutOfReach::Zero( query(), coreArena.blockSize);
            }
            else
            {
                Y_LOCK(giantLock);
                void *blockAddr = coreArena.acquireBlock();
                ++Coerce(allocated);
                return blockAddr;
            }
        }

        void Blanks:: storeBlank(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            assert(allocated>0);
            --Coerce(allocated);
            store( static_cast<BlankNode *>( OutOfReach::Zero(blockAddr,sizeof(BlankNode))) );
        }

        void Blanks:: eraseBlank(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            assert(allocated>0);
            --Coerce(allocated);
            Y_GIANT_LOCK();
            coreArena.releaseBlock(blockAddr);
        }


        size_t Blanks:: blockSize() const noexcept
        {
            return coreArena.blockSize;
        }


        
        void Blanks:: displayInfo(const size_t indent) const
        {
            static const char id[] = "Memory::Blanks";

            Core::Indent(std::cerr,indent) << "<" << id << " blockSize='" << coreArena.blockSize << "'>" << std::endl;
            Core::Indent(std::cerr,indent) << "  allocated = " << allocated << std::endl;
            Core::Indent(std::cerr,indent) << "  available = " << size      << std::endl;
            Core::Indent(std::cerr,indent) << "<" << id << "/>" << std::endl;
        }


        void Blanks:: gc(const size_t maxCount) noexcept
        {
            ListOf<BlankNode> data;
            Rework::PoolToList(data,*this);               // get data
            MergeSort::ByIncreasingAddress(data);         // sort by increasing address

            {
                Y_GIANT_LOCK();
                while(data.size>maxCount)
                    coreArena.releaseBlock( data.popTail() ); // return highest
            }
            while(data.size) store( data.popHead() );     // will use highest first
        }


    }
}

