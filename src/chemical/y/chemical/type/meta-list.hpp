
//! \file


#ifndef Y_Chemical_MetaList_Included
#define Y_Chemical_MetaList_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Proxy to ordered Small::List
        //
        //
        //______________________________________________________________________
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

            static inline bool AreDistinct(const MetaList &lhs,
                                           const MetaList &rhs) noexcept
            {
                for(const NodeType *node=lhs->head;node;node=node->next)
                {
                    if(rhs->has(**node)) return false;
                }
                return true;
            }

            static inline SignType Compare(const NodeType * const lhs, const NodeType *const rhs) noexcept
            {
                return Sign::Of( (**lhs).indx[TopLevel], (**rhs).indx[TopLevel] );
            }

            virtual void update() noexcept = 0; //!< post-insertion indexing

        private:
            Y_DISABLE_ASSIGN(MetaList);
            inline virtual typename
            SelfType::ConstInterface &surrogate() const noexcept { return list; }


        protected:

            //! after validation, insert parameter and update indices
            inline  void insert(Type &param)
            {
                assert(list.isStrictlySortedBy(Compare));
                ListOps::InsertOrdered(list,list.proxy->produce(param),Compare);
                assert(list.isStrictlySortedBy(Compare));
                update();
            }

            ListType list;
        };





        //! keep indexed up-to-date
        template<Level LEVEL, typename LIST>
        class CodingList : public MetaList<LIST>
        {
        public:
            typedef typename MetaList<LIST>::NodeType NodeType;
            typedef typename MetaList<LIST>::Type     Type;

        protected:
            inline explicit CodingList() noexcept : MetaList<LIST>() {
                Y_STATIC_CHECK(LEVEL!=TopLevel,BadLevel);
            }
            inline          CodingList(const CodingList &_)  : MetaList<LIST>(_) {}

        public:
            inline virtual ~CodingList() noexcept {}

            virtual void update() noexcept
            {
                size_t idx = 1;
                for(NodeType *node=this->list.head;node;node=node->next)
                    Coerce( (**node).indx[LEVEL] ) = idx++;
            }
        private:
            Y_DISABLE_ASSIGN(CodingList);

        };


        //! keep single instance of possible multiple parameter
        template <Level LEVEL, typename LIST>
        class ParaList : public CodingList<LEVEL,LIST>
        {
        public:
            typedef CodingList<LEVEL,LIST>       BaseList;
            typedef typename BaseList::Type      Type;
            typedef typename BaseList::NodeType  NodeType;

            inline explicit ParaList() noexcept : BaseList() {}
            inline virtual ~ParaList() noexcept {}
            inline ParaList(const ParaList &_) : BaseList(_) {}

            inline ParaList & operator<<(Type &param)
            {
                if(this->list.has(param)) return *this;
                this->insert(param);
                return *this;
            }

            inline void xch(ParaList &_) noexcept {
                this->list.swapWith(_.list);
            }

            void fusion(ParaList &other) noexcept
            {
                LIST &mine = this->list;
                LIST &peer = other.list;
                while(peer.size>0)
                {
                    NodeType * const node = peer.popHead();
                    if(mine.has(**node))
                        mine.proxy->destroy(node);
                    else
                        ListOps::InsertOrdered(mine,node,BaseList::Compare);
                }
                this->update();
            }

        private:
            Y_DISABLE_ASSIGN(ParaList);
        };


        //! keep single instance of possible single parameter
        template <Level LEVEL, typename LIST>
        class OrthoList : public CodingList<LEVEL,LIST>
        {
        public:
            typedef MetaList<LIST>           CoreList;
            typedef CodingList<LEVEL,LIST>   BaseList;
            typedef typename BaseList::Type  Type;

            inline explicit OrthoList() noexcept : BaseList() {}
            inline virtual ~OrthoList() noexcept {}
            inline OrthoList(const OrthoList &_) : BaseList(_) {}

            inline OrthoList & operator<<(Type &param)
            {
                assert( !this->list.has(param) );
                this->insert(param);
                return *this;
            }

            inline void xch(OrthoList &_) noexcept
            {
                this->list.swapWith(_.list);
            }

            //! fusion of two disting lists
            void fusion(OrthoList &rhs) noexcept
            {
                OrthoList &lhs = *this;
                assert( CoreList::AreDistinct(lhs,rhs) );
                ListOps::Fusion(lhs.list,rhs.list,CoreList::Compare);
                lhs.update();
            }


        private:
            Y_DISABLE_ASSIGN(OrthoList);
        };


        typedef ParaList<SubLevel,SList>  SubSList;
        typedef OrthoList<SubLevel,EList> SubEList;

    }

}

#endif
