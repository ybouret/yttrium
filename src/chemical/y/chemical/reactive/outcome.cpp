
#include "y/chemical/reactive/outcome.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {
        Outcome:: Outcome(const Situation     _st,
                          const Equilibrium & _eq,
                          const xreal_t       _eK,
                          XWritable &         _cc,
                          const Level     &   _lv,
                          const xreal_t       _xi) noexcept :
        st(_st),
        eq(_eq),
        eK(_eK),
        cc(_cc),
        lv(_lv),
        xi(_xi),
        ax(xi.abs()),
        sc(0)
        {
        }


        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        eq(_.eq),
        eK(_.eK),
        cc(_.cc),
        lv(_.lv),
        xi(_.xi),
        ax(_.ax),
        sc(_.sc)
        {}


        Outcome:: ~Outcome() noexcept
        {
        }


        const char * Outcome:: situation() const noexcept
        {
            return SituationText(st);
        }

        xreal_t Outcome:: affinity(XAdd &xadd, const XReadable &C, const Level L) const
        {
            return eq.affinity(eK,xadd,C,L);
        }

        std::ostream & operator<<(std::ostream &os, const Outcome &out)
        {
#if 0
            switch(out.st)
            {
                case Running: os << "(+)"; break;
                case Blocked: os << "(-)"; break;
                case Crucial: os << "(!)"; break;
            }
#endif
            os << out.situation();
            os << " @" << std::setw(24) << out.xi.str().c_str();
            os << ":"  << std::setw(24) << out.sc.str().c_str();
            os << ": " << out.eq.name;
            return os;
        }


    }

}


