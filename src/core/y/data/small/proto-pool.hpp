
//! \file

#ifndef Y_Data_Small_Proto_Pool_Included
#define Y_Data_Small_Proto_Pool_Included 1

#include "y/data/small/proto-linked.hpp"

namespace Yttrium
{

    namespace Small
    {
        template <typename NODE, template <typename> class PROXY>
        class ProtoPool : public ProtoLinked<PoolOf,NODE,PROXY>
        {
        public:
            typedef ProtoLinked<PoolOf,NODE,PROXY> ProtoType;
            typedef typename ProtoType::ProxyType  ProxyType;
            using ProtoType::proxy;
            using ProtoType::store;
            using ProtoType::release_;

            inline explicit ProtoPool() : ProtoType() {}
            inline explicit ProtoPool(const ProxyType &_) noexcept : ProtoType(_) {}
            inline virtual ~ProtoPool() noexcept {}
            inline ProtoPool(const ProtoPool &other) : ProtoType(other.proxy)
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                        store( proxy->replica(node) );
                    this->reverse();
                }
                catch(...) { release_(); throw; }
            }

        private:
            Y_DISABLE_ASSIGN(ProtoPool);
        };
    }

}

#endif

