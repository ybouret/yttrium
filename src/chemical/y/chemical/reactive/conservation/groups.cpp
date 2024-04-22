
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

                        
                    }
                }

            }

        }

    }

}
