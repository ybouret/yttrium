//! \file

#ifndef Y_Lingo_Logic_Or_Included
#define Y_Lingo_Logic_Or_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Or operands
        /**
         - stop at the first in-order operand, skipping only possible empty matches
         */
        //
        //
        //______________________________________________________________________
        class Or : public Logic
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('[','|', '|', ']'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Or() noexcept; //!< create empty
            virtual ~Or() noexcept; //!< cleanup
            Or(const Or &);         //!< copy

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()     const noexcept; //!< [Identifiable] CallSign
            virtual Pattern *    clone()                 const; //!< [Pattern] new Or(self)
            virtual bool         strong()       const noexcept; //!< [Pattern] size <=0  of all strong
            virtual bool         univocal()     const  ; //!< [Pattern] size == 1 and univocal
            virtual String       regularExpression()     const; //!< [Pattern] (first|second|...)
            virtual void         query(CharDB &fc)       const; //!< [Pattern] query all
            virtual bool         takes(Token &,Source &) const; //!< [Pattern]
            virtual void         viz(OutputStream &)     const; //!< [Pattern]

        private:
            Y_DISABLE_ASSIGN(Or);
        };
    }

}

#endif
