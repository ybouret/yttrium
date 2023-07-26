

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

            //! assign using own cache
            inline SoloLightList & operator=(const SoloLightList &other)
            {
                SoloLightList tmp;
                try {
                    for(const NodeType *node=other.head;node;node=node->next)
                        tmp.pushTail( this->proxy->replica(node) );
                }
                catch(...) { this->proxy->destruct(tmp); throw; }
                swapWith(tmp);
                return *this;
            }
        };
    }

}

#endif
