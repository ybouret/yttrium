
//! \file

#ifndef Y_Memory_Notes_Included
#define Y_Memory_Notes_Included 1

#include "y/memory/dyadic-arena.hpp"
#include "y/memory/pages.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Notes
        {
        public:
            static const unsigned MinShift = 0;
            static const unsigned MaxShift = Pages::MinShift-1;
            static const size_t   Number   = 1+MaxShift - MinShift; //!< number of possible dyadic arenas

            explicit Notes(Album &);
            virtual ~Notes() noexcept;

            DyadicArena &operator[](const size_t shift) noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Notes);
            static const size_t Required = Number*sizeof(DyadicArena);
            DyadicArena *dyad;
            void        *wksp[ Y_WORDS_GEQ(Required) ];

            void destruct(size_t built) noexcept;
        };
    }

}
#endif

