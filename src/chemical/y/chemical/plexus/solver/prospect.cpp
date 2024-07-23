#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Prospect:: ~Prospect() noexcept
        {
        }

        Prospect::  Prospect(const Equilibrium &_eq,
                             const xreal_t      _eK,
                             const xreal_t      _xi,
                             const XReadable   &_cc,
                             const XReadable   &_dc,
                             XWritable         &_dd) noexcept :
        eq(_eq),
        eK(_eK),
        xi(_xi),
        ax(xi.abs()),
        cc(_cc),
        dc(_dc),
        dd(_dd),
        sl(0)
        {

        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        eq(_.eq),
        eK(_.eK),
        xi(_.xi),
        ax(_.ax),
        cc(_.cc),
        dc(_.dc),
        dd(_.dd),
        sl(_.sl)
        {

        }

        int Prospect:: Compare(const Prospect &lhs,
                               const Prospect &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.ax, rhs.ax);
        }

        std::ostream & operator<<(std::ostream &os, const Prospect &pro)
        {
            os << pro.eq;
            return os;
        }


        void Prospect::  update(XAdd &xadd, XMul &xmul)
        {
            eq.drvsMassAction(eK, dd, SubLevel, cc, SubLevel, xmul);
            Coerce(sl) = eq.dot(dd, SubLevel,xadd);
        }


        size_t Prospect:: indx() const noexcept
        {
            return eq.indx[SubLevel];
        }

    }

}
