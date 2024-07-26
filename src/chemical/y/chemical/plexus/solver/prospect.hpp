//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Prospect
        {
        public:
            Prospect(const Equilibrium & _eq,
                     const xreal_t       _eK,
                     const XReadable    &_cc,
                     const xreal_t       _xi,
                     const xreal_t       _ks) noexcept;

            Prospect(const Prospect &_) noexcept;
            ~Prospect() noexcept;

            size_t sub() const noexcept;

            xreal_t objectiveFunction(const XReadable &C,
                                      const Level      L,
                                      XMul &           X) const;

            const Equilibrium &eq;
            const xreal_t      eK;
            const XReadable   &cc;
            const xreal_t      xi;
            const xreal_t      ks;
        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        typedef Small::CoopLightList<const Prospect> PList;
        typedef PList::NodeType                      PNode;
        typedef PList::ProxyType                     PBank;
    }

}

#endif

