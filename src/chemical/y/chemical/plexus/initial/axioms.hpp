
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
                
                explicit Axioms(const XCode &code); //!< setup from parsed design XNode
                Axioms(const Axioms &) noexcept;    //!< noexcept duplicate
                Y_OSTREAM_PROTO(Axioms);            //!< display

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

