
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
        ks(_ks)
        {

        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        eq(_.eq),
        eK(_.eK),
        cc(_.cc),
        xi(_.xi),
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
        
    }

}
