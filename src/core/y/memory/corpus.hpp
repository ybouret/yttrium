
//! \file

#ifndef Y_Memory_Corpus_Included
#define Y_Memory_Corpus_Included 1

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
        class Corpus
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
            explicit Corpus(Album &userAlbum); //!< setup from persistent Album
            virtual ~Corpus() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! access proper Dyad
            Dyad & operator[](const unsigned shift) noexcept;

            //! display statistics
            void displayInfo(const size_t indent) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Corpus);
            Album &album;
            Notes  notes;
        };
    }

}

#endif
