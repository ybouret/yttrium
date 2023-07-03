
//! \file

#ifndef Y_Memory_Guild_Included
#define Y_Memory_Guild_Included 1

#include "y/memory/arena.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Memory
    {
        
        //! arena facade
        template <typename T>
        class Guild
        {
        public:
            explicit Guild(Arena &userArena) noexcept :
            arena(userArena)
            {
                assert(arena.blockSize>=sizeof(T));
            }

            virtual ~Guild() noexcept;

            inline T   *zombie()                  { return static_cast<T*>(arena.acquire); }
            inline void zstore(T *dead) noexcept  { assert(0!=dead); arena.release( OutOfReach::Addr(dead) ); }

            inline void eradicate(T *live) noexcept { assert(0!=live); zstore( Destructed(live) ); }
            inline T   *construct() {
                void *blockAddr = arena.acquire();
                try { return new (blockAddr) T(); }
                catch(...) { arena.release(blockAddr); throw; }
            }


            Arena &arena;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guild);
        };

    }

}

#endif

