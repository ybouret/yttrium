
#include "y/chemical/reactive/outcome.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xReal Outcome:: activity(XMul &xmul, const xReal xi) const
        {
            return eq.activity(xmul, eK, C, L, xi);
        }


        Outcome:: Outcome(const Situation  &_st,
                                 const Components &_eq,
                                 const xReal       _eK,
                                 const XReadable &_C,
                                 const Level      _L) :
        st(_st),
        eq(_eq),
        eK(_eK),
        C(_C),
        L(_L)
        {

        }

        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        eq(_.eq),
        eK(_.eK),
        C(_.C),
        L(_.L)
        {

        }


        Outcome:: ~Outcome() noexcept {}
    }

}

