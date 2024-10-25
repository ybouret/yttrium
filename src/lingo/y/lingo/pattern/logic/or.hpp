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
            virtual void         viz(OutputStream &)     const; //!< [Pattern]

        private:
            Y_DISABLE_ASSIGN(Or);
            virtual void   _query(CharDB &fc)       const; //!< [Logic] query all
            virtual bool   _strong()                const; //!< [Logic] if all strong
            virtual String _regexp()                const; //!< [Logic] alternate
            virtual bool   _univocal()              const; //!< [Logic] size == 1 and univocal
            virtual bool   _takes(Token &,Source &) const; //!< [Logic] first one, try further if feeble

        };
    }

}

#endif
