
#include "y/apex/m/archon.hpp"
#include "y/memory/quarry.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/album.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        const char * const Archon:: CallSign = "Apex::Archon";

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
            Y_LOCK(Access);
            return engine->acquire(shift);
        }

        void Archon:: release(void *entry, const unsigned shift) noexcept
        {
            assert(0!=engine);
            assert(0!=entry);
            Y_LOCK(Access);
            engine->release(entry,shift);
        }

        void Archon:: CheckRequired(size_t   & bytes,
                                    unsigned & shift)
        {
            assert(0==shift);
            if(bytes>Base2<size_t>::MaxPowerOfTwo) throw Specific::Exception(CallSign,"too many required bytes");
            shift = Base2<size_t>::LogFor(bytes);
        }

        unsigned Archon:: ShiftFor(size_t &bytes)
        {
            if(bytes>Base2<size_t>::MaxPowerOfTwo) throw Specific::Exception(CallSign,"too many required bytes");
            return Base2<size_t>::LogFor(bytes);
        }

    }

}
