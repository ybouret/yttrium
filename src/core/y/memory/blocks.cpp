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
            {
                Slot *slot = slots+nslot;
                for(size_t i=nslot;i>0;--i)
                {
                    --slot;
                    while(slot->size>0)
                        build.release( Destructed(slot->popTail()) );
                    slot->~Slot();
                }
            }
            album[ Page::DefaultShift ].release(slots);
        }


        static inline size_t ComputeSlots()
        {
            Y_STATIC_CHECK(Page::DefaultBytes>=sizeof(Blocks::Slot),PageBytesTooSmall);
            return Base2<size_t>::One << (Page::DefaultShift - iLog2Of<Blocks::Slot>::Value);
        }

        Blocks:: Blocks(Album &userAlbum) :
        album(userAlbum),
        nslot( ComputeSlots() ),
        smask( nslot-1 ),
        cache(0),
        slots( static_cast<Slot *>(album[ Page::DefaultShift ].acquire()) ),
        which( &Blocks::acquireFirst ),
        build( sizeof(Arena), album, Page::DefaultBytes )
        {
            for(size_t i=0;i<nslot;++i) new (&slots[i]) Slot();
        }

        Arena * Blocks:: makeNewArena(const size_t blockSize)
        {
            assert(sizeof(Arena) == build.blockSize);
            void *blockAddr = build.acquire();
            try
            {
                return new (blockAddr) Arena(blockSize,album,Page::DefaultBytes);
            }
            catch(...)
            {
                build.release(blockAddr);
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
                Core::Indent(std::cerr,indent+2) << "<Slot index='" << i << "'>" << std::endl;
                for(const Arena *node=slot.head;node;node=node->next)
                {
                    node->displayInfo(indent+4);
                }
                Core::Indent(std::cerr,indent+2) << "<Slot/>" << std::endl;
            }
            Core::Indent(std::cerr,indent) << "<" << CallSign << "/>" << std::endl;
        }


        void * Blocks:: acquire(const size_t blockSize)
        {
            return ((*this).*which)(blockSize);
        }


        void *Blocks:: acquireFirst(const size_t blockSize)
        {
            assert(0==cache);
            cache = slots[blockSize&smask].pushHead(makeNewArena(blockSize));
            which = & Blocks::acquireExtra;
            return cache->acquire();
        }


        void *Blocks:: acquireExtra(const size_t blockSize)
        {
            assert(0!=cache);
            if(cache->blockSize==blockSize)
            {
                return cache->acquire();
            }
            else
            {
                Slot  &slot = slots[blockSize&smask];
                Arena *node = slot.head;
                for(;node;node=node->next)
                {
                    if(node->blockSize==blockSize)
                    {
                        return (cache = slot.moveToFront(node))->acquire();
                    }
                }
                assert(0==node);
                return (cache = slot.pushHead( makeNewArena(blockSize) ))->acquire();
            }
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
                cache->release(blockAddr);
            }
            else
            {
                Slot  &slot = slots[blockSize&smask];
                Arena *node = slot.head;
                for(;node;node=node->next)
                {
                    if(node->blockSize==blockSize)
                    {
                        return (cache = slot.moveToFront(node))->release(blockAddr);
                    }
                }
                Libc::CriticalError(EINVAL, "%s: corrupted release(blockSize=%lu)", CallSign, (unsigned long)blockSize);

            }
        }


    }

}

