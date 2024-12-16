#include "y/chemical/plexus/conservation/act.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            Act:: ~Act() noexcept {}

            Act::ConstInterface & Act:: surrogate() const noexcept { return my; }


            Act:: Act(const Law &first) :
            Proxy<const Law::Group>(),
            my(),
            next(0),
            prev(0)
            {
                my << first;
            }

            bool Act:: accepts(const Law &law) const noexcept
            {
                assert(!my.has(law));

                const Company &rhs = *law;
                for(const LNode *ln=my.head;ln;ln=ln->next)
                {
                    const Company &lhs = ***ln;
                    if(lhs.worksWith(rhs)) return true;
                }
                return false;
            }


            bool Act:: accepts(const Act &other) const noexcept
            {
                assert( this != &other );
                for(const LNode *ln=other->head;ln;ln=ln->next)
                {
                    const Law &law = **ln;
                    if(accepts(law)) return true;
                }
                return false;
            }




            void Act:: collect(const Law &law)
            {
                assert(!my.has(law));
                my << law;
            }

            void Act:: collect(const Act &act)
            {
                assert( this != &act );
                for(const LNode *ln=act->head;ln;ln=ln->next)
                    collect(**ln);
            }


        }

    }

}


