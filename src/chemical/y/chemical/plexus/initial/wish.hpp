
//! \file

#ifndef Y_Plexus_Initial_Wish_Included
#define Y_Plexus_Initial_Wish_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Wish
        {
        public:
            static const char * const CallSign;
            
            explicit Wish();
            virtual ~Wish() noexcept;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wish);
        };
    }
}

#endif

