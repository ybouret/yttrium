
//! \file

#ifndef Y_Field_Interface_Included
#define Y_Field_Interface_Included 1

#include "y/field/meta-key.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{
    namespace Field
    {

        class Interface : public Oversized, public Counted
        {
        public:
            virtual const MetaKey & key() const noexcept = 0;

            virtual ~Interface() noexcept;
        protected:
            explicit Interface() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Interface);
        };

    }

}

#endif
