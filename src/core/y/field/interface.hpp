
//! \file

#ifndef Y_Field_Interface_Included
#define Y_Field_Interface_Included 1

#include "y/field/meta-key.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{
    namespace Field
    {

        //______________________________________________________________________
        //
        //
        //
        //! Agnostic Field Interface
        //
        //
        //______________________________________________________________________
        class Interface : public Oversized, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const MetaKey & key() const noexcept = 0; //!< get metaKey

            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Interface() noexcept; //!< cleanup
        protected:
            explicit Interface() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Interface);
        };

    }

}

#endif
