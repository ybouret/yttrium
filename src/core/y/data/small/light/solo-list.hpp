

//! \file

#ifndef Y_Data_Small_Solo_Light_List_Included
#define Y_Data_Small_Solo_Light_List_Included 1

#include "y/data/small/light/list.hpp"
#include "y/data/small/proxy/solo.hpp"

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
        class SoloLightList : public LightList<T,SoloProxy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef LightNode<T>                  NodeType; //!< alias
            typedef LightList<T,SoloProxy>        ListType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup empty
            inline explicit SoloLightList() noexcept : ListType() {}

            //! copy
            inline explicit SoloLightList(const SoloLightList &_) : ListType(_) {}

            //! cleanup
            inline virtual ~SoloLightList() noexcept {}

        private:
            Y_DISABLE_ASSIGN(SoloLightList);
        };
    }

}

#endif
