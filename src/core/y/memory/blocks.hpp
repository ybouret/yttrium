//! \file

#ifndef Y_Memory_Blocks_Included
#define Y_Memory_Blocks_Included 1

#include "y/memory/arena.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Blocks
        {
        public:
            static const size_t       MinAvailable = 4;
            static const char * const CallSign;

            explicit Blocks(Album &);
            virtual ~Blocks() noexcept;

            void *acquire(const size_t blockSize);

            void expand();


        private:
            Album &      album;
            size_t       numArenas;
            size_t       available; //!< maximum number of arenas
            Arena *      acquiring; //!< acquiring cache
            Arena *      releasing; //!< releasing cache
            Pages *      dataPages; //!< memory owner
            Arena *      arenaHead; //!< arena[0]
            Arena *      arenaTail; //!< arena[count-1]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
        };

    }

}

#endif

