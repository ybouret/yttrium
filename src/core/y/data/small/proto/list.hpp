//! \file

#ifndef Y_Data_Small_Proto_List_Included
#define Y_Data_Small_Proto_List_Included 1

#include "y/data/small/proto/linked.hpp"
#include "y/data/list.hpp"
#include "y/container/iterator/linked.hpp"
#include "y/type/copy.hpp"

namespace Yttrium
{

    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! Prototype to handle lists with cache/proxy
        //
        //
        //______________________________________________________________________
        template <typename NODE, template <typename> class PROXY>
        class ProtoList : public ProtoLinked<ListOf<NODE>,NODE,PROXY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ProtoLinked<ListOf<NODE>,NODE,PROXY> ProtoType; //!< alias
            typedef typename ProtoType::ProxyType        ProxyType; //!< alias
            using ProtoType::proxy;
            using ProtoType::pushTail;
            using ProtoType::free_;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! default constructor
            inline explicit ProtoList() : ProtoType() {}

            //! constructor for [automatic|manual] proxy setup
            inline explicit ProtoList(const ProxyType &_) noexcept : ProtoType(_) {}

            //! cleanup
            inline virtual ~ProtoList() noexcept {}

            //! copy constructor, using proxy->replicat
            inline ProtoList(const ProtoList &other) : ProtoType(other.proxy)
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                        pushTail( proxy->replica(node) );
                }
                catch(...) { free_(); throw; }
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! destroy popHead
            inline void cutHead() noexcept { proxy->destroy( this->popHead() ); }

            //! destroy popTail
            inline void cutTail() noexcept { proxy->destroy( this->popTail() ); }

            //! destroy popNode
            inline void cutNode(NODE *node) noexcept
            {
                assert(0!=node);
                assert(this->owns(node));
                proxy->destroy( this->pop(node) );
            }

            //! add a copy of other
            inline void growTail(const CopyOf_ &, const ProtoList &other)
            {
                ProtoList temp(other);
                this->mergeTail(temp);
            }



            //__________________________________________________________________
            //
            //
            // Iterators
            //
            //__________________________________________________________________
            typedef Iterating::Linked<typename NODE::Type,NODE,Iterating::Forward>            Iterator;      //!< alias
            typedef Iterating::Linked<typename NODE::ConstType,const NODE,Iterating::Forward> ConstIterator; //!< alias
            inline  Iterator      begin()       noexcept { return Iterator(this->head);      }               //!< begin
            inline  Iterator      end()         noexcept { return Iterator(0);               }               //!< end
            inline  ConstIterator begin() const noexcept { return ConstIterator(this->head); }               //!< begin, const
            inline  ConstIterator end()   const noexcept { return ConstIterator(0);          }               //!< end, const

            typedef Iterating::Linked<typename NODE::Type,NODE,Iterating::Reverse>            ReverseIterator;      //!< alias
            typedef Iterating::Linked<typename NODE::ConstType,const NODE,Iterating::Reverse> ConstReverseIterator; //!< alias
            inline  ReverseIterator      rbegin()       noexcept { return ReverseIterator(this->tail);      }       //!< rbegin
            inline  ReverseIterator      rend()         noexcept { return ReverseIterator(0);               }       //!< rend
            inline  ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(this->tail); }       //!< rbegin, const
            inline  ConstReverseIterator rend()   const noexcept { return ConstReverseIterator(0);          }       //!< rend, const


        private:
            Y_DISABLE_ASSIGN(ProtoList);
        };
    }

}

#endif

