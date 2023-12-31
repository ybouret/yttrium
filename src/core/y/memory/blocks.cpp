#include "y/memory/blocks.hpp"
#include "y/memory/album.hpp"
#include "y/calculus/base2.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/check/static.hpp"

#include <cerrno>

namespace Yttrium
{

    namespace Memory
    {

        const char * const Blocks:: CallSign = "Memory::Blocks";

        Blocks:: ~Blocks() noexcept
        {
            // clean all slots
            {
                Slot *slot = slots+nslot;
                for(size_t i=nslot;i>0;--i)
                {
                    --slot;
                    while(slot->size>0)
                        build.releaseBlock( Destructed(slot->popTail()) );
                    slot->~Slot();
                }
            }

            // release slots
            album[ Page::DefaultShift ].release(slots);
            slots = 0;
            Coerce(nslot) = 0;
            Coerce(smask) = 0;
            cache         = 0;
            which         = 0;
        }


        static inline size_t ComputeSlots()
        {
            Y_STATIC_CHECK(Page::DefaultBytes>=sizeof(Blocks::Slot),PageBytesTooSmall);
            return Base2<size_t>::One << (Page::DefaultShift - iLog2Of<Blocks::Slot>::Value);
        }

        Blocks:: Blocks(Album &userAlbum) :
        album( userAlbum      ),
        nslot( ComputeSlots() ),
        smask( nslot-1        ),
        cache( 0              ),
        slots( static_cast<Slot *>(album[ Page::DefaultShift ].acquire()) ),
        which( &Blocks::acquireFirst ),
        build( sizeof(Arena), album )
        {
            // format slots
            for(size_t i=0;i<nslot;++i)
                new (&slots[i]) Slot();
        }

        Arena * Blocks:: makeNewArena(const size_t blockSize)
        {
            assert(sizeof(Arena) == build.blockSize);
            void *blockAddr = build.acquireBlock();
            try
            {
                return new (blockAddr) Arena(blockSize,album);
            }
            catch(...)
            {
                build.releaseBlock(blockAddr);
                throw;
            }
        }

        void  Blocks:: displayInfo(size_t indent) const
        {
            Core::Indent(std::cerr,indent) << "<" << CallSign << " slots='" << nslot << "'>" << std::endl;
            for(size_t i=0;i<nslot;++i)
            {
                const Slot &slot = slots[i];
                if(slot.size<=0) continue;
                if(slot.size>1) Core::Indent(std::cerr,indent+2) << "<Slot index='" << i << "' count='" << slot.size << "'>" << std::endl;
                for(const Arena *node=slot.head;node;node=node->next)
                {
                    node->displayInfo(indent+4,false);
                }
                if(slot.size>1) Core::Indent(std::cerr,indent+2) << "<Slot/>" << std::endl;
            }
            Core::Indent(std::cerr,indent) << "<" << CallSign << "/>" << std::endl;
        }


        void * Blocks:: acquire(const size_t blockSize)
        {
            assert(0!=which);
            return ((*this).*which)(blockSize);
        }


        Arena & Blocks:: operator[](const size_t blockSize)
        {
            assert(blockSize>0);
            
            if(0==cache || blockSize != cache->blockSize)
            {
                void *dummy = acquire(blockSize);
                release(dummy,blockSize);
            }

            assert(0!=cache);
            assert(blockSize==cache->blockSize);
            return *cache;

        }

        void *Blocks:: acquireFirst(const size_t blockSize)
        {
            // create first Arena and change methods
            assert(0==cache);
            cache = slots[blockSize&smask].pushHead(makeNewArena(blockSize));
            which = & Blocks::acquireExtra; // will never come back
            assert(blockSize==cache->blockSize);
            return cache->acquireBlock();
        }


        void *Blocks:: acquireExtra(const size_t blockSize)
        {
            assert(0!=cache);
            if(cache->blockSize==blockSize)
            {
                // cached!
                return cache->acquireBlock();
            }
            else
            {
                // look for cache in dedicated slot
                Slot  &slot = slots[blockSize&smask];
                Arena *node = slot.head;
                for(;node;node=node->next)
                {
                    if(node->blockSize==blockSize)
                    {
                        return (cache = slot.moveToFront(node) )->acquireBlock();
                    }
                }

                // create new arena
                assert(0==node);
                return (cache = slot.pushHead( makeNewArena(blockSize) ))->acquireBlock();
            }
        }

        bool Blocks:: owns(const void *blockAddr, const Arena * &theArena, const Chunk * &theChunk) const
        {
            assert(0!=blockAddr);
            assert(0==theArena);
            assert(0==theChunk);

            for(size_t i=0;i<nslot;++i)
            {
                for(const Arena *a = slots[i].head; a; a=a->next)
                {
                    if(a->owns(blockAddr,theChunk))
                    {
                        theArena = a;
                        return true;
                    }
                }
            }
            return false;

        }


    }

}

#include "y/system/error.hpp"

namespace Yttrium
{

    namespace Memory
    {


        void  Blocks:: release(void *blockAddr, const size_t blockSize) noexcept
        {
            assert(blockAddr);
            assert(blockSize>0);
            assert(0!=cache || Die("not previous acquire"));
            if(cache->blockSize==blockSize)
            {
                cache->releaseBlock(blockAddr);
            }
            else
            {
                Slot  &slot = slots[blockSize&smask];
                Arena *node = slot.head;
                for(;node;node=node->next)
                {
                    if(node->blockSize==blockSize)
                    {
                        return (cache = slot.moveToFront(node))->releaseBlock(blockAddr);
                    }
                }
                Libc::CriticalError(EINVAL, "%s: corrupted release(blockSize=%lu)", CallSign, (unsigned long)blockSize);

            }
        }


    }

}

