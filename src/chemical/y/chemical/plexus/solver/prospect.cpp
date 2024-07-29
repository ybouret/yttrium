
#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Prospect:: Prospect(const Equilibrium & _eq,
                            const xreal_t       _eK,
                            const XReadable    &_cc,
                            const xreal_t       _xi,
                            const xreal_t       _ks) noexcept :
        eq(_eq),
        eK(_eK),
        cc(_cc),
        xi(_xi),
        ax( xi.abs() ),
        ks(_ks)
        {

        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        eq(_.eq),
        eK(_.eK),
        cc(_.cc),
        xi(_.xi),
        ax(_.ax),
        ks(_.ks)
        {

        }

        Prospect:: ~Prospect() noexcept {}

        size_t Prospect:: sub() const noexcept { return eq.indx[SubLevel]; }


        xreal_t Prospect:: objectiveFunction(const XReadable &C,
                                             const Level      L,
                                             XMul &           X) const
        {
            const xreal_t value = ks * eq.massAction(eK, X, C, L);
            return value.abs();
        }


        std::ostream & Prospect:: display(std::ostream   &os,
                                          const Assembly &uuid,
                                          const bool      full) const
        {

            uuid.pad(os<< eq, eq);
            os << " | ax =" << std::setw(15) << real_t(ax);
            if(full)
            {
                eq.displayCompact(os << " @",cc, SubLevel);
                os << " ks=" << real_t(ks);
            }
            return os;
        }


    }

}
