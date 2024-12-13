
#include "y/chemical/plexus/conservation/conduct.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            Conduct:: ~Conduct() noexcept {}

            Conduct::ConstInterface & Conduct:: surrogate() const noexcept { return my; }


            Conduct:: Conduct(const Law &first) :
            Proxy<const Law::Group>(),
            my(),
            next(0),
            prev(0)
            {
                my << first;
            }

            bool Conduct:: accepts(const Law &law) const noexcept
            {
                assert(!my.has(law));

                const Company &rhs = *law;
                for(const Law::GNode *ln=my.head;ln;ln=ln->next)
                {
                    const Company &lhs = ***ln;
                    if(lhs.worksWith(rhs)) return true;
                }
                return false;
            }


            bool Conduct:: accepts(const Conduct &other) const noexcept
            {
                for(const Law::GNode *ln=other->head;ln;ln=ln->next)
                {
                    const Law &law = **ln;
                    if(accepts(law)) return true;
                }
                return false;
            }






        }

    }

}


