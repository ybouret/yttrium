
//! \file

#ifndef Y_Plexus_Initial_Design_Included
#define Y_Plexus_Initial_Design_Included 1

#include "y/chemical/plexus/initial/axiom.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            class Design : public Quantized, public Counted, public Entity
            {
            public:
                static const char * const CallSign;
                static const char * const LogoExpr;
                static const char * const Logo;

                typedef ArcPtr<const Design>      Pointer;
                typedef SuffixSet<String,Pointer> Set;

                explicit Design();
                virtual ~Design() noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Design);
            };
            
        }

    }
}

#endif

