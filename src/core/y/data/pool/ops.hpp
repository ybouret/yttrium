
//! \file

#ifndef Y_Data_Pool_Ops_Included
#define Y_Data_Pool_Ops_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Operations on Pool
    //
    //
    //__________________________________________________________________________
    struct PoolOps
    {

        //______________________________________________________________________
        //
        //! store node
        //______________________________________________________________________
        template <typename POOL, typename NODE> static
        inline NODE * Store(POOL &P, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);

            node->next = P.head;
            P.head = node;
            ++Coerce(P.size);
            return node;
        }

        //______________________________________________________________________
        //
        //! query (head) node
        //______________________________________________________________________
        template <typename POOL> static
        inline typename POOL::NodeType * Query(POOL &P) noexcept
        {
            assert(P.size>0);
            assert(0!=P.head);
            typename POOL::NodeType *node = P.head;
            P.head       = node->next;
            node->next   = 0; assert(0==node->prev);
            --Coerce(P.size);
            return node;
        }
        
    };

}

#endif

