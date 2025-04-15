
//! \file

#ifndef Y_Plexus_Initial_Design_Included
#define Y_Plexus_Initial_Design_Included 1

#include "y/chemical/plexus/initial/wish.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Design : public Quantized, public Counted, public Entity
        {
        public:
            static const char * const CallSign;
            
            explicit Design(const String * const pName) noexcept;
            virtual ~Design() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Design);
        };

    }
}

#endif

