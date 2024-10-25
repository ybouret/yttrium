
//! \file

#ifndef Y_Lingo_Logic_And_Included
#define Y_Lingo_Logic_And_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! And operands
        //
        //
        //______________________________________________________________________
        class And : public Logic
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('[','&', '&', ']'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit And() noexcept; //!< create empty
            virtual ~And() noexcept; //!< cleanup
            And(const And &);        //!< copy

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()     const noexcept; //!< [Identifiable] CallSign
            virtual Pattern *    clone()                 const; //!< [Pattern] new And(self)
            virtual bool         strong()       const noexcept; //!< [Pattern] at least one strong
            virtual bool         univocal()     const noexcept; //!< [Pattern] false if at least one feeble or multiple
            virtual String       regularExpression()     const; //!< [Pattern] (all)
            virtual void         query(CharDB &fc)       const; //!< [Pattern] query up to first strong
            virtual bool         takes(Token &,Source &) const; //!< [Pattern] all are taken
            virtual void         viz(OutputStream &)     const; //!< [Pattern]

        private:
            Y_DISABLE_ASSIGN(And);
        };
    }

}

#endif
