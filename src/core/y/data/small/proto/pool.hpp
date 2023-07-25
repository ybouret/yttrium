
//! \file

#ifndef Y_Data_Small_Proto_Pool_Included
#define Y_Data_Small_Proto_Pool_Included 1

#include "y/data/small/proto/linked.hpp"

namespace Yttrium
{

    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Prototype to handle pools with cache
        //
        //
        //______________________________________________________________________
        template <typename NODE, template <typename> class PROXY>
        class ProtoPool : public ProtoLinked<PoolOf,NODE,PROXY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ProtoLinked<PoolOf,NODE,PROXY> ProtoType; //!< alias
            typedef typename ProtoType::ProxyType  ProxyType; //!< alias
            using ProtoType::proxy;
            using ProtoType::store;
            using ProtoType::release_;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! default constructor
            inline explicit ProtoPool() : ProtoType() {}

            //! constructor for [automatic|manual] cache assignment
            inline explicit ProtoPool(const ProxyType &_) noexcept : ProtoType(_) {}

            //! cleanup
            inline virtual ~ProtoPool() noexcept {}

            //! copy constructor, using proxy
            inline ProtoPool(const ProtoPool &other) : ProtoType(other.proxy)
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                        store( proxy->replica(node) );
                    this->reverse();
                }
                catch(...) { release_(); throw; }
            }

        private:
            Y_DISABLE_ASSIGN(ProtoPool);
        };
    }

}

#endif

