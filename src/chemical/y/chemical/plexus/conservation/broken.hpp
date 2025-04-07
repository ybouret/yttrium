

//! \file

#ifndef Y_Chemical_Conservation_Broken_Included
#define Y_Chemical_Conservation_Broken_Included 1

#include "y/chemical/plexus/conservation/canon.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            class Broken
            {
            public:
                Broken(const xreal_t &_xs,
                       const Law     &_law) noexcept;
                ~Broken() noexcept;
                Broken(const Broken &) noexcept;

                //! show with canon for format
                std::ostream & show(std::ostream &, const Assembly &) const;

                const xreal_t  xs;
                const Law    &law;

            private:
                Y_DISABLE_ASSIGN(Broken);
            };


            typedef Small::CoopHeavyList<Broken> BList; //!< alias
            typedef BList::NodeType              BNode; //!< alias
            typedef BList::ProxyType             BBank; //!< alias

        }

    }

}

#endif

