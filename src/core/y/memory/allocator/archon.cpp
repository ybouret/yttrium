#include "y/memory/allocator/archon.hpp"
#include "y/memory/album.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/quarry.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Memory
    {

        const char * const Archon:: CallSign = "Memory::Archon";
        const unsigned     Archon:: MinShift = Quarry::MinShift;
        const size_t       Archon:: MinBytes = (size_t(1) << Quarry::MinShift);

        namespace
        {
            //__________________________________________________________________
            //
            //
            //! standalone dyadic allocator data
            //
            //__________________________________________________________________
            class CoreEngine
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline   CoreEngine() : album(), corpus(album) { }
                virtual ~CoreEngine() noexcept {}

                //______________________________________________________________
                //
                // Members
                //
                //______________________________________________________________
                Memory::Album  album;
                Memory::Corpus corpus;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CoreEngine);
            };

            struct ZBlock
            {
                typedef PoolOf<ZBlock> Pool;
                ZBlock *next;
            };
        }

        class Archon:: Engine : public CoreEngine
        {
        public:
            inline explicit Engine() : CoreEngine(), quarry(corpus), zbpool() { }
            inline virtual ~Engine() noexcept
            {
                while(zbpool.size>0)
                    quarry.release(zbpool.query(),MinShift);
            }
            
            inline void * acquire(unsigned &shift)
            {
                if(shift<=MinShift && zbpool.size>0)
                {
                    static const size_t one = 1;
                    shift = MinShift;
                    return memset( zbpool.query(), 0, one << shift);
                }
                else
                {
                    return quarry.acquire(shift);
                }
            }

            inline void release(void *entry, const unsigned shift) noexcept
            {
                if(shift<=MinShift)
                {
                    assert(MinShift==shift);
                    zbpool.store( static_cast<ZBlock *>(memset(entry,0,sizeof(ZBlock)) ) );
                }
                else
                    quarry.release(entry,shift);
            }



            Memory::Quarry        quarry;
            ZBlock::Pool          zbpool;

         private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };



        static Archon::Engine *engine = 0;
        static void           *engine_[ Y_WORDS_FOR(Archon::Engine) ];


        Archon:: ~Archon() noexcept
        {
            assert(0!=engine);
            Destruct(engine);
            engine = 0;
            Y_STATIC_ZARR(engine_);
        }

        Archon:: Archon() noexcept : Singleton<Archon>()
        {
            assert(0==engine);
            engine = new ( Y_STATIC_ZARR(engine_) ) Engine();
        }

        void * Archon:: acquire(unsigned &shift)
        {
            assert(0!=engine);
            Y_LOCK(access);
            void * const addr =  engine->acquire(shift);
            assert(shift>=MinShift);
            return addr;
        }

        void Archon:: release(void *entry, const unsigned shift) noexcept
        {
            assert(0!=engine);
            assert(0!=entry);
            Y_LOCK(access);
            engine->release(entry,shift);
        }


        void * Archon:: Acquire(unsigned &shift)
        {
            static Archon &self = Instance();
            return self.acquire(shift);
        }

        void Archon:: Release(void * entry, const unsigned shift) noexcept
        {
            assert(0!=entry);
            static Archon &self = Location();
            self.release(entry,shift);
        }

        size_t Archon:: EngineSize() noexcept
        {
            return sizeof(Engine);
        }
    }
}
