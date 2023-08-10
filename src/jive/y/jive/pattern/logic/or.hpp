//! \file

#ifndef Y_Jive_Pattern_Or_Included
#define Y_Jive_Pattern_Or_Included 1

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
        //! Logical Or: must match one of the patterns, in order
        //
        //
        //______________________________________________________________________
        class Or : public Compound
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('_','|','|','_'); //!< UUID

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Or()   noexcept; //!< setup empty
            explicit Or(const Or &);  //!< copy
            virtual ~Or()   noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern *clone() const;
            virtual bool     takes(Source &);
            virtual void     query(FirstChars &) const; //!< all first chars

            //! checking isFragile
            /**
             - isFragile if one of the patterns is fragile
             */
            virtual bool     isFragile() const noexcept;

        private:
            Y_DISABLE_ASSIGN(Or);
        };
    }

}

#endif
