

#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Prospect:: Prospect(const Situation     _st,
                            const Equilibrium & _eq,
                            const xreal_t       _ek,
                            XWritable &         _cc,
                            const xreal_t       _xi,
                            XWritable &         _dc) noexcept:
        st(_st),
        eq(_eq),
        ek(_ek),
        cc(_cc),
        xi(_xi),
        ax(xi.abs()),
        dc(_dc),
        ff(),
        ok(true)
        {
        }


        Prospect:: Prospect(const Prospect &_) noexcept :
        st(_.st),
        eq(_.eq),
        ek(_.ek),
        cc(_.cc),
        xi(_.xi),
        ax(_.ax),
        dc(_.dc),
        ff(_.ff),
        ok(_.ok)
        {
        }


        Prospect:: ~Prospect() noexcept {}


        std::ostream & Prospect:: show(std::ostream &os, const Cluster &cl, const XReadable * const Ktop) const

        {
            os << "|" << std::setw(15) << real_t(xi);
            os << "|" << Formatted::Get("%15.4g",real_t(ff));
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

        SignType Prospect:: CompareIncreasingFF(const Prospect &lhs, const Prospect &rhs) noexcept
        {
            if(lhs.ok)
            {
                // lhs.ok = true
                if(rhs.ok)
                {
                    // lhs.ok = true, rhs.ok = true
                    return Comparison::Increasing(lhs.ff, rhs.ff);
                }
                else
                {
                    // lhs.ok = true, rhs.ok = false
                    return Negative;
                }
            }
            else
            {
                // lhs.ok = false
                if(rhs.ok)
                {
                    // lhs.ok = false, rhs.ok = true
                    return Positive;
                }
                else
                {
                    // lhs.ok = false, rhs.ok = false
                    return Comparison::Increasing( lhs.eq.indx[TopLevel], rhs.eq.indx[TopLevel]);
                }
            }

        }

        SignType Prospect:: CompareDecreasingAX(const Prospect &lhs, const Prospect &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.ax, rhs.ax);
        }

        void Prospect:: step(XSwell &inc) const
        {
            assert(Running==st);
            eq.step(inc,dc);
        }


        String Prospect:: fileName() const
        {
            return (xi.abs().mantissa > 0 ? "good:" : "bad:") + eq.fileName() + ".pro";
        }
    }

}
