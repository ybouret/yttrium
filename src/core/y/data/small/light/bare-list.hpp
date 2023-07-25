
//! \file

#ifndef Y_Data_Small_Bare_Light_List_Included
#define Y_Data_Small_Bare_Light_List_Included 1

#include "y/data/small/light/list.hpp"
#include "y/data/small/proxy/bare.hpp"

namespace Yttrium
{

    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! Bare Light List : Light List with no cache
        //
        //
        //______________________________________________________________________
        template <typename T>
        class BareLightList : public LightList<T,BareProxy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef LightNode<T>                  NodeType; //!< alias
            typedef LightList<T,BareProxy>        ListType; //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup empty
            inline explicit BareLightList() noexcept: ListType() {}

            //! copy
            inline explicit BareLightList(const BareLightList &_) : ListType(_) {}

            //! cleanup
            inline virtual ~BareLightList() noexcept {}

        private:
            Y_DISABLE_ASSIGN(BareLightList);
        };
    }

}

#endif
