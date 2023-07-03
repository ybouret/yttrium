
//! \file

#ifndef Y_Memory_Guild_Included
#define Y_Memory_Guild_Included 1

#include "y/memory/arena.hpp"

namespace Yttrium
{

    namespace Memory
    {

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

            


            Arena &arena;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guild);
        };

    }

}

#endif

