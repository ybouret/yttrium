
//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Prospect
        {
        public:
            typedef CxxSeries<Prospect> Series;

            Prospect(const Situation     _st,
                     const Equilibrium & _eq,
                     const xreal_t       _ek,
                     const XReadable &   _cc,
                     const xreal_t       _xi) noexcept;
            Prospect(const Prospect &_) noexcept;
            ~Prospect() noexcept;

            std::ostream & show(std::ostream &os, const Cluster &cl, const XReadable &Ktop) const
            {
                os << std::setw(15) << real_t(xi) << " @";
                cl.display(os,eq,Ktop);
                return os;
            }

            xreal_t affinity(XMul            &X,
                             const XReadable &C,
                             const Level      L) const;

            static int Compare(const Prospect &lhs, const Prospect &rhs) noexcept;

            //! append xi * nu to species's incresase
            void step(XSwell &inc) const;


            const Situation     st;
            const Equilibrium & eq;
            const xreal_t       ek;
            const XReadable &   cc;
            const xreal_t       xi;
            const xreal_t       ax;

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };


        typedef Small::CoopLightList<const Prospect> PRepo;
        typedef PRepo::ProxyType                     PBank;
        typedef PRepo::NodeType                      PNode;

    }

}

#endif

