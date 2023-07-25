//! \file

#ifndef Y_Data_Small_Light_List_Included
#define Y_Data_Small_Light_List_Included 1

#include "y/data/small/proto/list.hpp"
#include "y/data/small/light/node.hpp"

namespace Yttrium
{

    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Light List API
        //
        //
        //______________________________________________________________________
        template <typename T, template <typename> class PROXY>
        class LightList : public ProtoList< LightNode<T>, PROXY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);                       //!< aliases
            typedef LightNode<T>              NodeType;  //!< alias
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
            //! setup default
            inline explicit LightList() : ProtoType() {}

            //! setup with proxy
            inline explicit LightList(const ProxyType &_) noexcept : ProtoType(_) {}

            //! copy
            inline explicit LightList(const LightList &_) : ProtoType(_) {}

        public:
            // cleanup
            inline virtual ~LightList() noexcept {}

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________

            //! pushTail
            LightList & operator<<( Type &args )
            {
                this->pushTail(proxy->produce(args));
                return *this;
            }

            //! pushHead
            LightList & operator>>( Type &args )
            {
                this->pushHead(proxy->produce(args));
                return *this;
            }




        private:
            Y_DISABLE_ASSIGN(LightList);
        };
    }

}

#endif

