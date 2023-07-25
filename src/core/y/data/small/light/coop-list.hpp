//! \file

#ifndef Y_Data_Small_Coop_Light_List_Included
#define Y_Data_Small_Coop_Light_List_Included 1

#include "y/data/small/light/list.hpp"
#include "y/data/small/proxy/coop.hpp"

namespace Yttrium
{

    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! Solo Light List : Light List with own cache
        //
        //
        //______________________________________________________________________
        template <typename T>
        class CoopLightList : public LightList<T,CoopProxy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef LightNode<T>                  NodeType;  //!< alias
            typedef LightList<T,CoopProxy>        ListType;  //!< alias
            typedef CoopProxy<NodeType>           ProxyType; //!< alias
            typedef typename ProxyType::CacheType CacheType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup empty with shared cache
            inline explicit CoopLightList(const ProxyType &sharedProxy) noexcept:
            ListType(sharedProxy) {}

            //! copy
            inline explicit CoopLightList(const CoopLightList &_) : ListType(_) {}

            //! cleanup
            inline virtual ~CoopLightList() noexcept {}

            CoopLightList & operator=(const CoopLightList &other)
            {
                CoopLightList tmp(other);
                tmp.swapWith(*this);
                return *this;
            }

        };
    }

}

#endif

