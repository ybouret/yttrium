
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
        out(_out),
        xi(_xi),
        ax(xi.abs()),
        dc(_dc)
        {
        }

        Prospect:: Prospect(const Prospect &_) noexcept :
        out(_.out),
        xi(_.xi),
        ax(_.ax),
        dc(_.dc)
        {
        }


    }

}

