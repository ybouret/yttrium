
//! \file

#ifndef Y_Plexus_Initial_Axiom_Included
#define Y_Plexus_Initial_Axiom_Included 1

#include "y/chemical/plexus/initial/player.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/chemical/type/xcode.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {
            class Axiom : public Quantized, public Entity, Proxy< const ListOf<Player> >
            {
            public:
                static const char * const CallSign;

                explicit Axiom(const XNode &root);
                virtual ~Axiom() noexcept;
                const xreal_t value;

                Axiom * next;
                Axiom * prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
                Y_PROXY_DECL();
                CxxListOf<Player> my;
            };
        }

    }

}

#endif

