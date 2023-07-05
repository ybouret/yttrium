
//! \file

#ifndef Y_Memory_Book_Included
#define Y_Memory_Book_Included 1

#include "y/config/starting.hpp"
#include "y/memory/notes.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! aggregates Album+Notes to cover 2^0 -> 2^MaxShift possibilities
        //
        //
        //______________________________________________________________________
        class Book
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned MinShift = 0;               //!< alias
            static const unsigned MidShift = Pages::MinShift; //!< alias
            static const unsigned MaxShift = Pages::MaxShift; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Book(Album &userAlbum); //!< setup from persistent Album
            virtual ~Book() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! access proper Dyad
            Dyad & operator[](const unsigned shift) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Book);
            Album &album;
            Notes  notes;
        };
    }

}

#endif
