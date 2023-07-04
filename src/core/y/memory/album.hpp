
//! \file

#ifndef Y_Memory_Album_Included
#define Y_Memory_Album_Included 1

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
        //! Album of all possible Pages
        //
        //
        //______________________________________________________________________
        class Album
        {
        public:
            //__________________________________________________________________
            //
            //
            //! Definitions
            //
            //__________________________________________________________________
            static const size_t       Required = Pages::Number * sizeof(Pages); //!< flat memory for all pages
            static const char * const CallSign;                                 //!< "Memory::Album"


            //__________________________________________________________________
            //
            //
            //  C++
            //
            //__________________________________________________________________
            explicit Album();                    //!< setup all empty pages
            explicit Album(Lockable &) noexcept; //!< setup all empty pages, using existing giant lock
            virtual ~Album()           noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! acess pages from MinShift to MaxShift
            //__________________________________________________________________
            Pages & operator[](const unsigned shift) noexcept;

            //__________________________________________________________________
            //
            //! display statistics
            //__________________________________________________________________
            void displayInfo(const size_t indent) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Album);
            Pages *pages;
            void  *pages_[Y_WORDS_GEQ(Required)];
            void   setupWith(Lockable &) noexcept;
        };
    }

}

#endif

