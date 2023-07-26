//! \file

#ifndef Y_Data_Small_Coop_Heavy_List_Included
#define Y_Data_Small_Coop_Heavy_List_Included 1

#include "y/data/small/heavy/list.hpp"
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
        class CoopHeavyList : public HeavyList<T,CoopProxy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef HeavyNode<T>                  NodeType;  //!< alias
            typedef HeavyList<T,CoopProxy>        ListType;  //!< alias
            typedef CoopProxy<NodeType>           ProxyType; //!< alias
            typedef typename ProxyType::CacheType CacheType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup empty with shared cache
            inline explicit CoopHeavyList(const ProxyType &sharedProxy) noexcept:
            ListType(sharedProxy) {}

            //! copy
            inline explicit CoopHeavyList(const CoopHeavyList &_) : ListType(_) {}

            //! cleanup
            inline virtual ~CoopHeavyList() noexcept {}

            //! assign
            CoopHeavyList & operator=(const CoopHeavyList &other)
            {
                CoopHeavyList tmp(other);
                tmp.swapWith(*this);
                return *this;
            }
        };
    }

}

#endif

