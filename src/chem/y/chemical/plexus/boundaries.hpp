
//! \file

#ifndef Y_Chemical_Boundaries_Included
#define Y_Chemical_Boundaries_Included 1

#include "y/chemical/plexus/boundary.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small:: CoopHeavyList<Boundary> BList;
        typedef BList:: ProxyType               BBank;
        typedef BList:: NodeType                BNode;

        class Boundaries : public Proxy<const BList>
        {
        public:
            explicit Boundaries(const BBank &bbank) noexcept;
            virtual ~Boundaries()                   noexcept;

            void add(const Species &s,
                     const xReal    x);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundaries);
            virtual ConstInterface & surrogate() const noexcept;
            
            BList list;
        };
    }

}

#endif
