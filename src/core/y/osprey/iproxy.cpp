
#include "y/osprey/iproxy.hpp"

namespace Yttrium
{
    namespace Osprey
    {


        IProxy:: ~IProxy() noexcept
        {
        }

        IProxy:: IProxy(const IProxy &_) :
        Proxy<const IList>(),
        my(_.my)
        {}


        IProxy:: IProxy(const IBank &bank) noexcept :
        Proxy<const IList>(),
        my(bank)
        {}

        bool IProxy:: tryRemove(const size_t indx) noexcept
        {
            for(INode *node=my.head;node;node=node->next)
            {
                if( indx == **node )
                {
                    my.cutNode(node);
                    return true;
                }
            }
            return false;
        }

        Y_PROXY_IMPL(IProxy,my)

        
    }
}

