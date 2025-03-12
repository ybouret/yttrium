
#include "y/chemical/type/species-list.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        Y_PROXY_IMPL(OrderedSList,list)

        OrderedSList:: OrderedSList() noexcept :
        list()
        {
        }

        OrderedSList:: ~OrderedSList() noexcept
        {
            
        }

        SignType OrderedSList:: Compare(const SNode * const lhs, const SNode * const rhs) noexcept
        {
            return Sign::Of( (**lhs).indx[TopLevel], (**rhs).indx[TopLevel] );
        }

        OrderedSList & OrderedSList:: operator<<(const Species &sp)
        {
            if(list.has(sp)) return *this;
            ListOps::InsertOrdered(list,list.proxy->produce(sp),Compare);
            update();
            return *this;
        }

        void OrderedSList:: collect(const Components &eq)
        {
            OrderedSList &self = *this;
            for(Components::ConstIterator it=eq->begin();it!=eq->end();++it)
                self << (*it)->sp;
        }


    }
}
