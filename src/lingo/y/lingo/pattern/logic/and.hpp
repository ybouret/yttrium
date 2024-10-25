
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
            virtual void         viz(OutputStream &)     const; //!< [Pattern]

        private:
            Y_DISABLE_ASSIGN(And);
            virtual void   _query(CharDB &fc)        const; //!< [Logic] query up to first strong
            virtual bool   _strong()                 const; //!< [Logic] at least one strong
            virtual String _regexp()                 const; //!< [Logic] catenate
            virtual bool   _univocal()               const; //!< [Logic] false if at least one feeble or multiple
            virtual bool   _takes(Token &, Source &) const; //!< [Logic] all are taken
        };
    }

}

#endif
