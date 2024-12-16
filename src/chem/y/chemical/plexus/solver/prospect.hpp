//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/reactive/equilibrium/outcome.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Prospect : public Entity
        {
        public:
            explicit Prospect(const Outcome &   _out,
                     const xReal       _xi,
                     const XReadable & _dx) noexcept;
            explicit Prospect(const Prospect &) noexcept;
            virtual ~Prospect() noexcept;
            Y_OSTREAM_PROTO(Prospect);

            virtual const String & key() const noexcept;

            const Outcome     out;   //!< current outcome
            const xReal       xi;    //!< current extent to outcome
            const xReal       ax;    //!< |xi|
            const XReadable & dc;    //!< SubLevel deltaC

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        typedef Small::CoopHeavyList<const Prospect> ProList;
        typedef ProList::NodeType                    ProNode;
        typedef ProList::ProxyType                   ProBank;

    }

}

#endif
