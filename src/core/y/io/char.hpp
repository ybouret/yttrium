//! \file

#ifndef Y_IOS_Char_Included
#define Y_IOS_Char_Included 1

#include "y/memory/exclusive/decl.hpp"

namespace Yttrium
{
    namespace IO
    {
        //______________________________________________________________________
        //
        //
        //
        //! Dedicated Char for dyamic Input/Output
        //
        //
        //______________________________________________________________________
        class Char
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Char(const uint8_t) noexcept; //!< setup
            Char(const Char &)  noexcept; //!< copy
            ~Char()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            uint8_t &       operator*()       noexcept;  //!< access
            const uint8_t & operator*() const noexcept;  //!< access, const

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Char *next; //!< for list/pool
            Char *prev; //!< for list


            //__________________________________________________________________
            //
            //! managed by a Memory::Studio
            //__________________________________________________________________
            Y_EXCLUSIVE_DECL();

        private:
            Y_DISABLE_ASSIGN(Char);
            uint8_t data;

        };
    }
}

#endif

