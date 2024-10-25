//! \file

#ifndef Y_Lingo_Logic_None_Included
#define Y_Lingo_Logic_None_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! None operands
        //
        //
        //______________________________________________________________________
        class None : public Logic
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('[','^', '^', ']'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit None() noexcept; //!< setup empty
            virtual ~None() noexcept; //!< cleanup
            None(const None &);       //!< copy

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()     const noexcept; //!< [Identifiable] CallSign
            virtual Pattern *    clone()                 const; //!< [Pattern] new None(self)
            virtual void         viz(OutputStream &)     const; //!< [Pattern]

        private:
            Y_DISABLE_ASSIGN(None);
            virtual void   _query(CharDB &fc)       const; //!< [Logic]
            virtual bool   _strong()                const; //!< [Logic]
            virtual String _regexp()                const; //!< [Logic] [^catenate]
            virtual bool   _univocal()              const; //!< [Logic]
            virtual bool   _takes(Token &,Source &) const; //!< [Logic]

        };
    }

}

#endif
