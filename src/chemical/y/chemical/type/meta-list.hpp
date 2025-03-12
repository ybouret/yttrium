
//! \file


#ifndef Y_Chemical_MetaList_Included
#define Y_Chemical_MetaList_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/check/static.hpp"

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


        protected:
            inline explicit MetaList() noexcept : SelfType(), list() {}
            inline          MetaList(const MetaList &_) : SelfType(), list(_.list) {}

        public:
            inline virtual ~MetaList() noexcept {}

            inline MetaList & operator<<(Type &param)
            {
                if(list.has(param)) return *this;
                ListOps::InsertOrdered(list,list.proxy->produce(param),Compare);
                assert(list.isSortedAccordingTo(Compare));
                update();
                return *this;
            }

            inline void xch(MetaList &other) noexcept
            {
                list.swapWith(other.list);
            }

            static inline SignType Compare(const NodeType * const lhs, const NodeType *const rhs) noexcept
            {
                return Sign::Of( (**lhs).indx[TopLevel], (**rhs).indx[TopLevel] );
            }

        private:
            Y_DISABLE_ASSIGN(MetaList);
            inline virtual typename
            SelfType::ConstInterface &surrogate() const noexcept { return list; }

            virtual void update() noexcept = 0;

        protected:
            ListType list;
        };

        template<Level LEVEL, typename LIST>
        class ParaList : public MetaList<LIST>
        {
        public:
            typedef typename MetaList<LIST>::NodeType NodeType;

            inline explicit ParaList() noexcept : MetaList<LIST>() {
                Y_STATIC_CHECK(LEVEL!=TopLevel,BadLevel);
            }
            inline virtual ~ParaList() noexcept {}
            inline          ParaList(const ParaList &_)  : MetaList<LIST>(_) {}



        private:
            Y_DISABLE_ASSIGN(ParaList);
            virtual void update() noexcept
            {
                size_t sub = 1;
                for(NodeType *node=this->list.head;node;node=node->next)
                    Coerce( (**node).indx[LEVEL] ) = sub++;
            }

        };


        typedef ParaList<SubLevel,SList> SubSList;
        typedef ParaList<SubLevel,EList> SubEList;

    }

}

#endif
