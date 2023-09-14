
//! \file

#ifndef Y_Jive_Pattern_And_Included
#define Y_Jive_Pattern_And_Included 1

#include "y/jive/pattern/logic/compound.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! Logical AND: must match all patterns
        //
        //
        //______________________________________________________________________
        class And : public Compound
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('A','N','D','_'); //!< UUID

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit And()   noexcept; //!< setup empty
            explicit And(const And &); //!< copy
            virtual ~And()   noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern *clone() const;
            virtual bool     takes(Source &);
            virtual void     query(FirstChars &) const;    //! first chars up to first regular
            virtual bool     isFragile() const noexcept;   //!<iff is empty or all patterns are fragile
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual void     viz(OutputStream &fp)      const;
            virtual bool     isUnivocal() const noexcept; //!< iff all patterns are univocal
            
        private:
            Y_DISABLE_ASSIGN(And);
        };
    }

}

#endif
