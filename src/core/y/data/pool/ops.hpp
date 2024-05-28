
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
        template <typename POOL, typename NODE> static inline
        NODE * Store(POOL &P, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);

            node->next = P.head;
            P.head = node;
            ++Coerce(P.size);
            return node;
        }

        //______________________________________________________________________
        //
        //! stash node (slow)
        //______________________________________________________________________
        template <typename POOL, typename NODE> static inline
        NODE *Stash(POOL &P, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            if(P.size<=0)
                return Store(P,node);
            else
            {
                NODE  *curr = P.head; assert(0!=curr);
                while(0!=curr->next) curr = curr->next;
                curr->next = node;
                ++Coerce(P.size);
                return node;
            }
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
            node->next   = 0; 
            --Coerce(P.size);
            return node;
        }

        //______________________________________________________________________
        //
        //! query [1..size] node
        //______________________________________________________________________
        template <typename POOL> static inline
        typename POOL::NodeType *Query(POOL &P, size_t indx) noexcept
        {
            assert(indx>=1);
            assert(indx<=P.size);
            if(1==indx)
            {
                // special case
                return Query(P);
            }
            else
            {
                // look up
                typedef typename POOL::NodeType NodeType;
                NodeType *node = P.head;
                NodeType *prev = 0;
                while(--indx>0)
                {
                    assert(0!=node);
                    prev = node;
                    node = node->next;
                }
                assert(NULL!=prev);
                NodeType *next = node->next;
                if(0!=next)
                {
                    // not at the end
                    prev->next = next;
                    node->next = 0;
                }
                else
                {
                    // at the end
                    prev->next = 0;
                }
                --Coerce(P.size);
                return node;
            }

        }

        
    };

}

#endif

