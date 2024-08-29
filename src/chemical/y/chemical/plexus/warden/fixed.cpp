
#include "y/chemical/plexus/warden/fixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fixed:: Fixed(XWritable &              _cc,
              const Conservation::Law &_cl) noexcept :
        gg(),
        cc(_cc),
        cl(_cl)
        {
        }

        Fixed:: Fixed(const Fixed &_) noexcept :
        gg(_.gg),
        cc(_.cc),
        cl(_.cl)
        {
        }

        Fixed:: ~Fixed() noexcept {}

        std::ostream & operator<<( std::ostream &os, const Fixed &self)
        {
            os << std::setw(15) << real_t(self.gg) << " @" << self.cl << " -> ";
            self.cl.displayCompact(os,self.cc, SubLevel);
            return os;
        }


        bool Fixed:: still(const XReadable &C,
                   const Level      L,
                   XAdd            &xadd)
        {
            return cl.broken(gg,cc,SubLevel,C,L,xadd);
        }

        int Fixed:: Compare(const Fixed &lhs, const Fixed &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.gg, rhs.gg);
        }

    }

}

