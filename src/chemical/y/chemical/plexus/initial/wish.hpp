
//! \file

#ifndef Y_Plexus_Initial_Wish_Included
#define Y_Plexus_Initial_Wish_Included 1

#include "y/chemical/plexus/initial/axiom.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {
            

            class Wish : public Quantized, public Entity
            {
            public:
                static const char * const CallSign;
                explicit Wish();
                virtual ~Wish() noexcept;
                
                
                const xreal_t target;
                Wish * next;
                Wish * prev;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wish);
            };
        }
    }
}

#endif

