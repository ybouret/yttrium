//! \file

#ifndef Y_Data_Small_Proto_List_Included
#define Y_Data_Small_Proto_List_Included 1

#include "y/data/small/proto-linked.hpp"

namespace Yttrium
{

    namespace Small
    {
        template <typename NODE, template <typename> class PROXY>
        class ProtoList : public ProtoLinked<ListOf,NODE,PROXY>
        {
        public:
            typedef ProtoLinked<ListOf,NODE,PROXY> ProtoType;
            typedef typename ProtoType::ProxyType  ProxyType;
            using ProtoType::proxy;
            using ProtoType::pushTail;
            using ProtoType::release_;

            inline explicit ProtoList() : ProtoType() {}
            inline explicit ProtoList(const ProxyType &_) noexcept : ProtoType(_) {}
            inline virtual ~ProtoList() noexcept {}
            inline ProtoList(const ProtoList &other) : ProtoType(other.proxy)
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                        pushTail( proxy->replica(node) );
                }
                catch(...) { release_(); throw; }
            }

        private:
            Y_DISABLE_ASSIGN(ProtoList);
        };
    }

}

#endif

