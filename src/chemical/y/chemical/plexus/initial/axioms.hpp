
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
            //__________________________________________________________________
            //
            //
            //
            //! store pre-compiled axioms for design
            //
            //
            //__________________________________________________________________
            class Axioms : public Entity, public Proxy<const XList>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef SuffixSet<String,const Axioms> Set; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Axioms(const XCode &code); //!< setup from parsed design XNode
                Axioms(const Axioms &) noexcept;    //!< noexcept duplicate
                virtual ~Axioms() noexcept;         //!< cleanup
                Y_OSTREAM_PROTO(Axioms);            //!< display

            private:
                XCode xcode;
                Y_PROXY_DECL();
            };
        }
    }
}

#endif

