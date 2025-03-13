
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


            static inline SignType Compare(const NodeType * const lhs, const NodeType *const rhs) noexcept
            {
                return Sign::Of( (**lhs).indx[TopLevel], (**rhs).indx[TopLevel] );
            }

        private:
            Y_DISABLE_ASSIGN(MetaList);
            inline virtual typename
            SelfType::ConstInterface &surrogate() const noexcept { return list; }

            virtual void update() noexcept = 0; //!< post-insertion indexing

        protected:
            inline  void insert(Type &param)
            {
                assert(list.isSortedAccordingTo(Compare));
                ListOps::InsertOrdered(list,list.proxy->produce(param),Compare);
                assert(list.isSortedAccordingTo(Compare));
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


        private:
            Y_DISABLE_ASSIGN(CodingList);
            virtual void update() noexcept
            {
                size_t idx = 1;
                for(NodeType *node=this->list.head;node;node=node->next)
                    Coerce( (**node).indx[LEVEL] ) = idx++;
            }
        };


        //! keep single instance of possible multiple parameter
        template <Level LEVEL, typename LIST>
        class ParaList : public CodingList<LEVEL,LIST>
        {
        public:
            typedef CodingList<LEVEL,LIST> MyList;
            typedef typename MyList::Type  Type;

            inline explicit ParaList() noexcept : MyList() {}
            inline virtual ~ParaList() noexcept {}
            inline ParaList(const ParaList &_) : MyList(_) {}

            inline ParaList & operator<<(Type &param)
            {
                if(this->list.has(param)) return *this;
                this->insert(param);
                return *this;
            }

            inline void xch(ParaList &_) noexcept
            {
                this->list.swapWith(_.list);
            }

        private:
            Y_DISABLE_ASSIGN(ParaList);
        };


        //! keep single instance of possible single parameter
        template <Level LEVEL, typename LIST>
        class OrthoList : public CodingList<LEVEL,LIST>
        {
        public:
            typedef CodingList<LEVEL,LIST> MyList;
            typedef typename MyList::Type  Type;

            inline explicit OrthoList() noexcept : MyList() {}
            inline virtual ~OrthoList() noexcept {}
            inline OrthoList(const OrthoList &_) : MyList(_) {}

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

        private:
            Y_DISABLE_ASSIGN(OrthoList);
        };

        
        typedef ParaList<SubLevel,SList>  SubSList;
        typedef OrthoList<SubLevel,EList> SubEList;

    }

}

#endif
