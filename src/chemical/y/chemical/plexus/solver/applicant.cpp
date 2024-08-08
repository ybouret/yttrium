
#include "y/chemical/plexus/solver/applicant.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Applicant::  Applicant(const Equilibrium &  _eq,
                               const xreal_t        _eK,
                               const XReadable    & _cc,
                               const xreal_t        _xi) noexcept :
        eq(_eq),
        eK(_eK),
        cc(_cc),
        xi(_xi),
        ax( xi.abs() ),
        ff(0)
        {
        }


        Applicant:: Applicant(const Applicant &_) noexcept :
        eq(_.eq),
        eK(_.eK),
        cc(_.cc),
        xi(_.xi),
        ax(_.ax),
        ff(_.ff)
        {
        }

        size_t Applicant:: isub() const noexcept { return eq.indx[SubLevel]; }

        real_t Applicant:: affinity(XMul            &X,
                                    const XReadable &C,
                                    const Level      L) const
        {
            return eq.affinity(eK, X, C, L);
        }


        int Applicant:: CompareAX(const Applicant &lhs, const Applicant &rhs) noexcept
        {
            return Comparison::Decreasing<xreal_t>(lhs.ax,rhs.ax);
        }

        int Applicant:: CompareFF(const Applicant &lhs, const Applicant &rhs) noexcept
        {
            return Comparison::Increasing<xreal_t>(lhs.ff,rhs.ff);
        }

        std::ostream & Applicant:: display(std::ostream   &os,
                                           const Assembly &uuid,
                                           const bool      full) const
        {
            uuid.pad(os<< eq, eq);
            os << "|ax =" << std::setw(15) << real_t(ax);
            os << "|ff =" << std::setw(15) << real_t(ff);
            if(full)
            {
                eq.displayCompact(os << " @",cc, SubLevel);
            }
            return os;
        }
    }

}
