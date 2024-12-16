
#include "y/chemical/plexus/solver/prospect.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Prospect:: ~Prospect() noexcept
        {
        }

        Prospect:: Prospect(const Outcome & _out,
                            const xReal     _xi,
                            const XReadable & _dc) noexcept :
        Entity(),
        out(_out),
        xi(_xi),
        ax(xi.abs()),
        dc(_dc),
        ff(0)
        {
        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        Entity(),
        out(_.out),
        xi(_.xi),
        ax(_.ax),
        dc(_.dc),
        ff(_.ff)
        {
        }

        const String & Prospect:: key() const noexcept
        {
            return out.eq.name;
        }

        xReal Prospect:: score(XMul &X, const XReadable &C, const Level &L) const
        {
            return out.eq.score(X,out.eK,C,L);
        }
        
        



    }

}





