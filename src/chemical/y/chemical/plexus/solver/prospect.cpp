#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Prospect:: ~Prospect() noexcept
        {
        }

        Prospect::  Prospect(const Equilibrium &_eq,
                             const xreal_t      _xi,
                             const XReadable   &_cc,
                             const XReadable   &_dd) noexcept :
        eq(_eq),
        xi(_xi),
        ax(xi.abs()),
        cc(_cc),
        dd(_dd),
        sl(0)
        {

        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        eq(_.eq),
        xi(_.xi),
        ax(_.ax),
        cc(_.cc),
        dd(_.dd),
        sl(_.sl)
        {

        }

        int Prospect:: Compare(const Prospect &lhs,
                               const Prospect &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.ax, rhs.ax);
        }
    }

}
