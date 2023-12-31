
//! \file

#ifndef Y_Data_Small_Heavy_List_Included
#define Y_Data_Small_Heavy_List_Included 1

#include "y/data/small/proto/list.hpp"
#include "y/data/small/heavy/node.hpp"

namespace Yttrium
{

    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Heavy List API
        //
        //
        //______________________________________________________________________
        template <typename T, template <typename> class PROXY>
        class HeavyList : public ProtoList< HeavyNode<T>, PROXY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type);                         //!< aliases
            typedef HeavyNode<T>              NodeType;  //!< alias
            typedef ProtoList<NodeType,PROXY> ProtoType; //!< alias
            typedef PROXY<NodeType>           ProxyType; //!< alias
            using ProtoType::proxy;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup empty
            inline explicit HeavyList() : ProtoType() {}

            //! setup from proxy
            inline explicit HeavyList(const ProxyType &_) noexcept : ProtoType(_) {}

            //! copy
            inline explicit HeavyList(const HeavyList &_) : ProtoType(_) {}

        public:
            //! cleanup
            inline virtual ~HeavyList() noexcept {}

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________

            //! push at tail
            inline HeavyList & operator<<(ParamType args)
            {
                this->pushTail( proxy->produce(args) );
                return *this;
            }

            //! push at head
            inline HeavyList & operator>>(ParamType args)
            {
                this->pushHead( proxy->produce(args) );
                return *this;
            }

            //! return hard copy of head content and remove head
            inline Type pullHead()
            {
                assert(this->size>0);
                assert(0!=this->head);
                ConstType res = **(this->head);
                this->cutHead();
                return res;
            }

            //! return hard copy of head content and remove head
            inline Type pullTail()
            {
                assert(this->size>0);
                assert(0!=this->tail);
                ConstType res = **(this->tail);
                this->cutTail();
                return res;
            }






        private:
            Y_DISABLE_ASSIGN(HeavyList);
        };

    }

}

#endif

