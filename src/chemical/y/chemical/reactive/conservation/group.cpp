
#include "y/chemical/reactive/conservation/group.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Group:: ~Group() noexcept
            {
            }

            Group:: Group(const Law &first) :
            Object(),
            clList(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }


            bool Group:: accepts(const Law &law) const noexcept
            {
                for(const clNode *cl=head;cl;cl=cl->next)
                {
                    const Law &mine = **cl;
                    if(mine.sharesSpeciesWith(law)) return true;
                }
                return false;
            }


            bool Group:: accepts(const Group &group) const noexcept
            {
                for(const clNode *cl=head;cl;cl=cl->next)
                {
                    if(group.accepts(**cl)) return true;
                }
                return false;
            }


        }

    }

}
