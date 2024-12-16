
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
                            XWritable &     _dc) noexcept :
        Outcome(_out),
        xi(_xi),
        ax(xi.abs()),
        dc(_dc),
        ff(0)
        {
        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        Outcome(_),
        xi(_.xi),
        ax(_.ax),
        dc(_.dc),
        ff(_.ff)
        {
        }



        xReal Prospect:: score(XMul &X, const XReadable &C, const Level &L) const
        {
            return eq.score(X,eK,C,L);
        }
        
        



    }

}





