
//! \file


#ifndef Y_Chemical_MetaList_Included
#define Y_Chemical_MetaList_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/check/static.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Proxy to ordered Small::List by TopLevel index
        //
        //
        //______________________________________________________________________
        template <typename LIST>
        class MetaList : public Proxy<const LIST>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef LIST                         ListType; //!< alias
            typedef Proxy<const ListType>        SelfType; //!< alias
            typedef typename ListType::NodeType  NodeType; //!< alias
            typedef typename ListType::Type      Type;     //!< alias

        protected:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit MetaList() noexcept :          SelfType(), list()      {} //!< setup
            inline          MetaList(const MetaList &_) : SelfType(), list(_.list) {} //!< duplicate

        public:
            inline virtual ~MetaList() noexcept {} //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check distinct lists
            static inline bool AreDistinct(const MetaList &lhs,
                                           const MetaList &rhs) noexcept
            {
                for(const NodeType *node=lhs->head;node;node=node->next)
                {
                    if(rhs->has(**node)) return false;
                }
                return true;
            }

            
            //! comparison by TopLevel index
            static inline SignType Compare(const NodeType * const lhs, const NodeType *const rhs) noexcept
            {
                return Sign::Of( (**lhs).indx[TopLevel], (**rhs).indx[TopLevel] );
            }

            //! sorting by TopLevel index for any acceptable list
            static inline void Sort(LIST &L) { MergeSort::Call(L,Compare); }


            //! post-insertion indexing
            virtual void update() noexcept = 0;

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const MetaList &self)
            {
                os << '{';
                const NodeType *node=self.list.head;
                if(node) {
                    os << (**node).name;
                    for(node=node->next;node;node=node->next)
                        os << ',' << (**node).name;
                }
                os << '}';
                return os;
            }

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

            //! implementation
            ListType list;
        };




        //______________________________________________________________________
        //
        //
        //
        //! keep indexed[LEVEL] up-to-date
        //
        //
        //______________________________________________________________________
        template<Level LEVEL, typename LIST>
        class CodingList : public MetaList<LIST>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename MetaList<LIST>::NodeType NodeType; //!< alias
            typedef typename MetaList<LIST>::Type     Type;     //!< alias

        protected:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            inline explicit CodingList() noexcept :
            MetaList<LIST>()
            {
                Y_STATIC_CHECK(LEVEL!=TopLevel,BadLevel);
            }

            //! duplicate
            inline CodingList(const CodingList &_)  : MetaList<LIST>(_) {}

        public:
            //! cleanup
            inline virtual ~CodingList() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! set proper indices after a change
            inline virtual void update() noexcept
            {
                size_t idx = 1;
                for(NodeType *node=this->list.head;node;node=node->next) {
                    Coerce( (**node).indx[LEVEL] ) = idx++;
                    std::cerr << "coding " << (**node).name << " => " << (**node).indx[LEVEL] << std::endl;
                }
            }
        private:
            Y_DISABLE_ASSIGN(CodingList);

        };

        //______________________________________________________________________
        //
        //
        //
        //! keep single instance of possible multiple parameter
        //
        //
        //______________________________________________________________________
        template <Level LEVEL, typename LIST>
        class ParaList : public CodingList<LEVEL,LIST>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CodingList<LEVEL,LIST>       BaseList; //!< alias
            typedef typename BaseList::Type      Type;     //!< alias
            typedef typename BaseList::NodeType  NodeType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit ParaList() noexcept : BaseList() {} //!< setup
            inline virtual ~ParaList() noexcept {}              //!< cleanup
            inline ParaList(const ParaList &_) : BaseList(_) {} //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert param, drop if multiple
            inline ParaList & operator<<(Type &param)
            {
                if(this->list.has(param)) return *this;
                this->insert(param);
                return *this;
            }

            //! no-throw exchange
            inline void xch(ParaList &_) noexcept {
                this->list.swapWith(_.list);
            }

            //! incremental fusion
            inline void fusion(ParaList &other) noexcept
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef MetaList<LIST>           CoreList; //!< alias
            typedef CodingList<LEVEL,LIST>   BaseList; //!< alias
            typedef typename BaseList::Type  Type;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit OrthoList() noexcept : BaseList() {}   //!< setup
            inline virtual ~OrthoList() noexcept {}                //!< cleanup
            inline OrthoList(const OrthoList &_) : BaseList(_) {}  //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert param that MUST NOT exist
            inline OrthoList & operator<<(Type &param)
            {
                assert( !this->list.has(param) );
                this->insert(param);
                return *this;
            }

            //! no-throw exchange
            inline void xch(OrthoList &_) noexcept
            {
                this->list.swapWith(_.list);
            }

            //! fusion of two disting lists
            inline void fusion(OrthoList &rhs) noexcept
            {
                OrthoList &lhs = *this;
                assert( CoreList::AreDistinct(lhs,rhs) );
                ListOps::Fusion(lhs.list,rhs.list,CoreList::Compare);
                lhs.update();
            }

            inline void free() noexcept
            {
                this->list.free();
            }

        private:
            Y_DISABLE_ASSIGN(OrthoList);
        };


        typedef ParaList<SubLevel,SList>  SubSList; //!< alias
        typedef OrthoList<SubLevel,EList> SubEList; //!< alias
        
    }

}

#endif
