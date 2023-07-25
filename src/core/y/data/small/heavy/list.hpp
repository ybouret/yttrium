
//! \file

#ifndef Y_Data_Small_Heavy_List_Included
#define Y_Data_Small_Heavy_List_Included 1

#include "y/data/small/proto/list.hpp"
#include "y/data/small/heavy/node.hpp"

namespace Yttrium
{

    namespace Small
    {
        template <typename T, template <typename> class PROXY>
        class HeavyList : public ProtoList< HeavyNode<T>, PROXY>
        {
        public:
            typedef HeavyNode<T>              NodeType;
            typedef ProtoList<NodeType,PROXY> ProtoType;
            typedef PROXY<NodeType>           ProxyType;

        protected:
            inline explicit HeavyList() : ProtoType() {}
            inline explicit HeavyList(const ProxyType &_) noexcept : ProtoType(_) {}
            inline explicit HeavyList(const HeavyList &_) : ProtoType(_) {}

        public:
            inline virtual ~HeavyList() noexcept {}

        private:
            Y_DISABLE_ASSIGN(HeavyList);
        };

    }

}

#endif

