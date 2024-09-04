

#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Prospect:: Prospect(const Situation     _st,
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


        Prospect:: Prospect(const Prospect &_) noexcept :
        st(_.st),
        eq(_.eq),
        ek(_.ek),
        cc(_.cc),
        xi(_.xi),
        ax(_.ax)
        {
        }


        Prospect:: ~Prospect() noexcept {}


        std::ostream & Prospect:: show(std::ostream &os, const Cluster &cl, const XReadable &Ktop) const

        {
            os << std::setw(15) << real_t(xi) << " @";
            cl.display(os,eq,Ktop);
            return os;
        }


        xreal_t Prospect:: affinity(XMul            &X,
                                    const XReadable &C,
                                    const Level      L) const
        {
            return eq.affinity(ek,X,C,L);
        }

        int Prospect:: Compare(const Prospect &lhs, const Prospect &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.ax, rhs.ax);
        }

        void Prospect:: step(XSwell &sw) const
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

    }

}
