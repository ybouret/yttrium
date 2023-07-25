//! \file

#ifndef Y_Data_Small_Proto_List_Included
#define Y_Data_Small_Proto_List_Included 1

#include "y/data/small/proto/linked.hpp"

namespace Yttrium
{

    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! Prototype to handle lists with cache
        //
        //
        //______________________________________________________________________
        template <typename NODE, template <typename> class PROXY>
        class ProtoList : public ProtoLinked<ListOf,NODE,PROXY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ProtoLinked<ListOf,NODE,PROXY> ProtoType; //!< alias
            typedef typename ProtoType::ProxyType  ProxyType; //!< alias
            using ProtoType::proxy;
            using ProtoType::pushTail;
            using ProtoType::release_;

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

            //! copy constructor, using proxy
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

