
//! \file


#ifndef Y_Chemical_Initial_Axioms_Included
#define Y_Chemical_Initial_Axioms_Included 1

#include "y/chemical/type/xcode.hpp"
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            //! store pre-compiled axioms for design
            class Axioms : public Entity, public Proxy<const XList>
            {
            public:
                typedef SuffixSet<String,const Axioms> Set;

                //! setup from parsed design XNode
                explicit Axioms(const XCode &code);
                Axioms(const Axioms &) noexcept;
                
                //! cleanup
                virtual ~Axioms() noexcept;
                
            private:
                XCode xcode;
                Y_PROXY_DECL();
            };
        }
    }
}

#endif

