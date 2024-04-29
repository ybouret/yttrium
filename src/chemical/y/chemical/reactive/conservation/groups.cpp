
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
                            group->append(*law);
                            fusion();
                            goto DONE;
                        }
                    }

                    // greate new group;
                    cgl.pushTail( new Group(*law) );
                DONE:
                    continue;
                }

                
                for(Group *group=cgl.head;group;group=group->next)
                {
                    group->compile();
                    for(const LawNode *ln=(*group)->head;ln;ln=ln->next)
                    {
                        const Law &law = **ln;
                        assert(law.alpha.size()==law.beta.cols);
                        assert(law.alpha.size()==law.beta.rows);
                        assert(law.alpha.size()==group->species.size);

                    }
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
                            lhs->append(*rhs);
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
