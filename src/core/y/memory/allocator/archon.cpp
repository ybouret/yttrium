#include "y/memory/allocator/archon.hpp"
#include "y/memory/album.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/quarry.hpp"

namespace Yttrium
{
    namespace Memory
    {

        const char * const Archon:: CallSign = "Memory::Archon";
        const unsigned     Archon:: MinShift = Quarry::MinShift;
        const size_t       Archon:: MinBytes = (size_t(1) << Quarry::MinShift);

        namespace
        {
            class CoreEngine
            {
            public:
                inline   CoreEngine() : album(), corpus(album) { }
                virtual ~CoreEngine() noexcept {}

                Memory::Album  album;
                Memory::Corpus corpus;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CoreEngine);
            };
        }

        class Archon:: Engine : public CoreEngine, public Memory::Quarry
        {
        public:
            explicit Engine() : CoreEngine(), Memory::Quarry(corpus)
            {
            }
            virtual ~Engine() noexcept {
            }

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


    }
}
