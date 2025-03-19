
#include "y/chemical/reactive/outcome.hpp"

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
        xi(_xi)
        {
        }


        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        eq(_.eq),
        eK(_.eK),
        cc(_.cc),
        lv(_.lv),
        xi(_.xi)
        {}


        Outcome:: ~Outcome() noexcept
        {
        }

    }

}


