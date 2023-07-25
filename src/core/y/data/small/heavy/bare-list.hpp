
//! \file

#ifndef Y_Data_Small_Bare_Heavy_List_Included
#define Y_Data_Small_Bare_Heavy_List_Included 1

#include "y/data/small/heavy/list.hpp"
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
        class BareHeavyList : public HeavyList<T,BareProxy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef HeavyNode<T>                 NodeType; //!< alias
            typedef HeavyList<T,BareProxy>       ListType; //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup empty
            inline explicit BareHeavyList() noexcept: ListType() {}

            //! copy
            inline explicit BareHeavyList(const BareHeavyList &_) : ListType(_) {}

            //! cleanup
            inline virtual ~BareHeavyList() noexcept {}

        private:
            Y_DISABLE_ASSIGN(BareHeavyList);
        };
    }

}

#endif
