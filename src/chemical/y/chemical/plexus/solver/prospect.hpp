
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

            explicit Prospect(const Situation     _st,
                              const Equilibrium & _eq,
                              const xreal_t       _ek,
                              const XReadable &   _cc,
                              const xreal_t       _xi) noexcept:
            st(_st),
            eq(_eq),
            ek(_ek),
            cc(_cc),
            xi(_xi),
            ax(xi.abs())
            {
            }

            Prospect(const Prospect &_) noexcept :
            st(_.st),
            eq(_.eq),
            ek(_.ek),
            cc(_.cc),
            xi(_.xi),
            ax(_.ax)
            {
            }

            ~Prospect() noexcept {}

            std::ostream & show(std::ostream &os, const Cluster &cl, const XReadable &Ktop) const
            {
                os << std::setw(15) << real_t(xi) << " @";
                cl.display(os,eq,Ktop);
                return os;
            }

            xreal_t affinity(XMul            &X,
                             const XReadable &C,
                             const Level      L) const
            {
                return eq.affinity(ek,X,C,L);
            }

            static int Compare(const Prospect &lhs, const Prospect &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.ax, rhs.ax);
            }

            void step(XSwell &sw) const
            {
                size_t             nc  = eq->size();
                for(Equilibrium::ConstIterator it=eq->begin();nc>0;--nc,++it)
                {
                    const Component     &cm = **it;
                    const Species       &sp = cm.sp;
                    const size_t * const id = sp.indx;

                    sw[ id[SubLevel] ] << cm.xn * xi;
                }
            }


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

