
//! \file

#ifndef Y_Memory_Book_Included
#define Y_Memory_Book_Included 1

#include "y/config/starting.hpp"
#include "y/memory/notes.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Book
        {
        public:
            static const unsigned MinShift = 0;
            static const unsigned MidShift = Pages::MinShift;
            static const unsigned MaxShift = Pages::MaxShift;

            explicit Book(Album &userAlbum);
            virtual ~Book() noexcept;

            Dyad & operator[](const unsigned shift) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Book);
            Album &album;
            Notes  notes;
        };
    }

}

#endif
