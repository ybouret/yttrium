
#include "y/chemical/reactive/conservation/groups.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Groups:: Groups( ) noexcept : cgl()
            {

            }

            Groups:: ~Groups() noexcept
            {
            }

            Groups::ConstInterface & Groups::surrogate() const noexcept
            {
                return cgl;
            }

            void Groups:: collect(const Laws &laws)
            {
                cgl.release();
                for(const Law *law = laws->head; law; law=law->next)
                {

                    for(Group *group=cgl.head;group;group=group->next)
                    {
                        if(group->accepts(*law))
                        {
                            (*group) << *law;
                            fusion();
                            goto DONE;
                        }
                    }

                    // greate new group;
                    cgl.pushTail( new Group(*law) );
                DONE:
                    continue;
                }
            }


            void Groups:: fusion() noexcept
            {
                Group::List store;
                while(cgl.size>0)
                {
                    AutoPtr<Group> rhs = cgl.popHead();
                    for(Group *lhs=store.head;lhs;lhs=lhs->next)
                    {
                        if(lhs->accepts(*rhs))
                        {
                            lhs->mergeTail(*rhs);
                            goto DONE;
                        }
                    }
                    store.pushTail(rhs.yield());
                DONE:
                    continue;
                }

                cgl.swapWith(store);
            }


        }

    }

}
