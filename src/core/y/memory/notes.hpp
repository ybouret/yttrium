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
        //______________________________________________________________________
        //
        //
        //
        //! small blockSize DyadicArenas
        //
        //
        //______________________________________________________________________
        class Notes
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned MinShift = 0;                     //!< MinBlockSize = 1
            static const unsigned MaxShift = Pages::MinShift-1;     //!< MaxBlockSize = 2^MaxShift
            static const size_t   Number   = 1+MaxShift - MinShift; //!< number of possible dyadic arenas

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Notes(Album &);    //!< setup with album
            virtual ~Notes() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            DyadicArena &operator[](const size_t shift) noexcept; //!< access arena
            void displayInfo(const size_t indent)          const; //!< display Statistics

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

