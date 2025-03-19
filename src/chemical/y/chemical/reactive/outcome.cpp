
#include "y/chemical/reactive/outcome.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {
        Outcome:: Outcome(const Situation    _st,
                          const Components & _eq,
                          const xreal_t      _eK,
                          const XReadable &  _cc,
                          const Level     &  _lv,
                          const xreal_t      _xi) noexcept :
        st(_st),
        eq(_eq),
        eK(_eK),
        cc(_cc),
        lv(_lv),
        xi(_xi),
        ax(xi.abs())
        {
        }


        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        eq(_.eq),
        eK(_.eK),
        cc(_.cc),
        lv(_.lv),
        xi(_.xi),
        ax(_.ax)
        {}


        Outcome:: ~Outcome() noexcept
        {
        }


        const char * Outcome:: situation() const noexcept
        {
            return SituationText(st);
        }

        std::ostream & operator<<(std::ostream &os, const Outcome &out)
        {
            switch(out.st)
            {
                case Running: os << "(+)"; break;
                case Blocked: os << "(-)"; break;
                case Crucial: os << "(!)"; break;
            }
            os << ' ' << out.situation() << ' ';
            os << " @" << std::setw(24) << out.xi.str().c_str();
            os << ": " << out.eq.name;
            return os;
        }


    }

}


