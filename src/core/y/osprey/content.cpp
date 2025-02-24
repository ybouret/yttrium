
#include "y/osprey/content.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        Content:: Content(const IBank &bank,
                          const size_t indx)   :
        IProxy(bank)
        {
            my << indx;
        }

        Content:: Content(const Content &_) :
        IProxy(_)
        {
        }
        

        Content:: Content(const Content &root,
                         const size_t   indx) :
        IProxy(root)
        {
            *this << indx;
            assert(root->size+1==my.size);
        }

        Content:: ~Content() noexcept
        {
        }

        Content & Content:: operator<<(const size_t indx) {
            assert( !my.has(indx) );
            ListOps::InsertOrdered(my,my.proxy->produce(indx), Compare);
            return *this;
        }

        SignType Content:: Compare(const INode *lhs, const INode *rhs) noexcept
        {
            return Sign::Of(**lhs,**rhs);
        }

        bool Content:: includes(const Content &sub) const noexcept
        {
            for(const INode *node=sub->head;node;node=node->next)
            {
                if( !my.has(**node) ) return false;
            }
            return true;
        }
    }

}


#include "y/osprey/residue.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        Content & Content:: operator<<(Residue &residue)
        {
            while( residue->size > 0)
                ListOps::InsertOrdered(my,residue.my.popHead(),Compare);
            return *this;
        }
    }

}
