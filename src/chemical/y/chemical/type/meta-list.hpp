
//! \file


#ifndef Y_Chemical_MetaList_Included
#define Y_Chemical_MetaList_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        template <typename LIST>
        class MetaList : public Proxy<const LIST>
        {
        public:
            typedef LIST                         ListType;
            typedef Proxy<const ListType>        SelfType;
            typedef typename ListType::NodeType  NodeType;
            typedef typename ListType::Type      Type;

            explicit MetaList() noexcept : SelfType(), list() {}
            virtual ~MetaList() noexcept {}

            inline MetaList & operator<<(Type &param)
            {
                if(list.has(param)) return *this;
                
                return *this;
            }


            static inline SignType Compare(const NodeType * const lhs, const NodeType *const rhs) noexcept
            {
                return Sign::Of( (**lhs).indx[TopLevel], (**rhs).indx[TopLevel] );
            }
        private:
            Y_DISABLE_ASSIGN(MetaList);
            inline virtual typename
            SelfType::ConstInterface &surrogate() const noexcept
            {
                return list;
            }

            ListType list;
        };
    }

}

#endif
