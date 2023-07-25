//! \file

#ifndef Y_Data_Small_Light_List_Included
#define Y_Data_Small_Light_List_Included 1

#include "y/data/small/proto/list.hpp"
#include "y/data/small/light/node.hpp"

namespace Yttrium
{

    namespace Small
    {
        template <typename T, template <typename> class PROXY>
        class LightList : public ProtoList< LightNode<T>, PROXY>
        {
        public:
            typedef LightNode<T>              NodeType;
            typedef ProtoList<NodeType,PROXY> ProtoType;
            typedef PROXY<NodeType>           ProxyType;

        protected:
            inline explicit LightList() : ProtoType() {}
            inline explicit LightList(const ProxyType &_) noexcept : ProtoType(_) {}
            inline explicit LightList(const LightList &_) : ProtoType(_) {}

        public:
            inline virtual ~LightList() noexcept {}

        private:
            Y_DISABLE_ASSIGN(LightList);
        };
    }

}

#endif

