

//! \file

#ifndef Y_Data_Small_Solo_Heavy_List_Included
#define Y_Data_Small_Solo_Heavy_List_Included 1

#include "y/data/small/heavy/list.hpp"
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
        class SoloHeavyList : public HeavyList<T,SoloProxy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef HeavyNode<T>                  NodeType; //!< alias
            typedef HeavyList<T,SoloProxy>        ListType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup empty
            inline explicit SoloHeavyList() noexcept : ListType() {}

            //! copy
            inline explicit SoloHeavyList(const SoloHeavyList &_) : ListType(_) {}

            //! cleanup
            inline virtual ~SoloHeavyList() noexcept {}

            //! assign
            inline SoloHeavyList & operator=(const SoloHeavyList &other)
            {
                SoloHeavyList tmp;
                try {
                    for(const NodeType *node=other.head;node;node=node->next)
                    {
                        tmp.pushTail( this->proxy->replica(node) );
                    }
                }
                catch(...)
                {
                    this->proxy->destruct(tmp); throw;
                }
                swapWith(tmp);
                return *this;
            }

        };
    }

}

#endif
