
#include "y/chemical/plexus/warden/trade.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Trade:: Trade(const Equilibrium & _eq,
                      const XReadable &   _cc,
                      const xreal_t       _gg,
                      const XReadable &   _dc //, const size_t        _nc
                      ) noexcept :
        eq(_eq),
        cc(_cc),
        gg(_gg),
        dc(_dc) //, nc(_nc)
        {}

        Trade:: ~Trade() noexcept
        {
        }

        Trade:: Trade(const Trade &_) noexcept :
        eq(_.eq),
        cc(_.cc),
        gg(_.gg),
        dc(_.dc) //, nc(_.nc)
        {
        }


        std::ostream & operator<<( std::ostream &os, const Trade &tr)
        {
            os << std::setw(15) << real_t(tr.gg) << " @" << tr.eq;
            tr.eq.displayCompact(os << ' ', tr.cc, SubLevel);
            return os;
        }

        int Trade:: Compare(const Trade &lhs, const Trade &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.gg,rhs.gg);
        }
    }

}
