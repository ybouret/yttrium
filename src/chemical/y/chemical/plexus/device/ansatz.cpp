#include "y/chemical/plexus/device/ansatz.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Ansatz:: step(XSwell &inc) const
        {
            assert(Running==st);
            eq.step(inc,dc);
        }


        xreal_t Ansatz:: objectiveFunction(XMul &X, const XReadable &C, const Level L) const
        {
            return eq.affinity(ek,X,C,L);
        }

        std::ostream & operator<<(std::ostream &os, const Ansatz &ans)
        {

            os << "|ff=" << Formatted::Get("%15.4g",real_t(ans.ff));
            os << "|xi=" << std::setw(15) << real_t(ans.xi);
            os << " @" << ans.eq;
            return os;
        }


        SignType Ansatz:: DecreasingAX(const Ansatz &lhs, const Ansatz &rhs) noexcept
        {
            assert(Crucial==lhs.st);
            assert(Crucial==rhs.st);
            return Sign::Of(rhs.ax,lhs.ax);
        }

        SignType Ansatz:: IncreasingFF(const Ansatz &lhs, const Ansatz &rhs) noexcept
        {
            assert(Running==lhs.st); assert(lhs.ff.mantissa>=0);
            assert(Running==rhs.st); assert(rhs.ff.mantissa>=0);
            return Sign::Of(lhs.ff,rhs.ff);
        }

        Ansatz:: Ansatz(const Equilibrium &_eq,
                        const xreal_t      _ek,
                        const Situation    _st,
                        XWritable &        _cc,
                        const xreal_t      _xi,
                        XWritable &        _dc) noexcept :
        eq(_eq),
        ek(_ek),
        st(_st),
        cc(_cc),
        xi(_xi),
        ax(xi.abs()),
        dc(_dc),
        ff(0.0),
        ok(false)
        {
        }

        Ansatz:: Ansatz(const Ansatz &_) noexcept :
        eq(_.eq),
        ek(_.ek),
        st(_.st),
        cc(_.cc),
        xi(_.xi),
        ax(_.ax),
        dc(_.dc),
        ff(_.ff),
        ok(_.ok)
        {

        }

        Ansatz:: ~Ansatz() noexcept {}

    }

}
