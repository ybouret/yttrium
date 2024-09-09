

#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Prospect:: Prospect(const Situation     _st,
                            const Equilibrium & _eq,
                            const xreal_t       _ek,
                            const XReadable &   _cc,
                            const xreal_t       _xi,
                            const XReadable &   _dc) noexcept:
        st(_st),
        eq(_eq),
        ek(_ek),
        cc(_cc),
        xi(_xi),
        dc(_dc),
        ax(xi.abs()),
        ff()
        {
        }


        Prospect:: Prospect(const Prospect &_) noexcept :
        st(_.st),
        eq(_.eq),
        ek(_.ek),
        cc(_.cc),
        xi(_.xi),
        dc(_.dc),
        ax(_.ax),
        ff(_.ff)
        {
        }


        Prospect:: ~Prospect() noexcept {}


        std::ostream & Prospect:: show(std::ostream &os, const Cluster &cl, const XReadable * const Ktop) const

        {
            os << "|" << std::setw(15) << real_t(xi);
            os << "|" << Formatted::Get("%15.4f",real_t(ff));
            os << "|"; 
            if(0!=Ktop)
            {
                cl.display(os,eq,*Ktop);
            }
            else
            {
                os << eq;
            }
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
            //return Comparison::CxxDecreasing(lhs.ax, rhs.ax);
            return Comparison::CxxIncreasing(lhs.ff, rhs.ff);
        }

        void Prospect:: step(XSwell &inc) const
        {
            assert(Running==st);
            eq.step(inc,xi);
        }

    }

}
