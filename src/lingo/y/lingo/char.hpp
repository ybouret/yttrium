//! \file

#ifndef Y_Lingo_Char_Included
#define Y_Lingo_Char_Included 1

#include "y/lingo/context.hpp"
#include "y/memory/exclusive/decl.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Char with full Context
        //
        //
        //______________________________________________________________________
        class Char : public Context
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Char> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Char(const Context &, const uint8_t) noexcept; //!< setup
            virtual ~Char()                               noexcept; //!< cleanup
            Char(const Char &)                            noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            uint8_t &       operator*()       noexcept; //!< r/w access
            const uint8_t & operator*() const noexcept; //!< r/o access
            const char    * printable() const noexcept; //!< printable for token


            
            //__________________________________________________________________
            //
            //
            //  Members
            //
            //__________________________________________________________________
            Char *  next; //!< for token
            Char *  prev; //!< for token

            //__________________________________________________________________
            //
            //! managed by a Memory::Studio
            //__________________________________________________________________
            Y_EXCLUSIVE_DECL();

        private:
            uint8_t data;
            Y_DISABLE_ASSIGN(Char);
        };
    }
}

#endif

