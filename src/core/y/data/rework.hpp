//! \file

#ifndef Y_Data_Rework_Included
#define Y_Data_Rework_Included 1

#include "y/data/pool.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Reworking linked objects
    //
    //
    //__________________________________________________________________________

    struct Rework
    {
        //______________________________________________________________________
        //
        //
        //! convert a pool into a list which must be initially empty
        //
        //______________________________________________________________________
        template <typename NODE> static inline
        void PoolToList(ListOf<NODE> &L, PoolOf<NODE> &P) noexcept
        {
            assert(0==L.size);
            assert(0==L.head);
            assert(0==L.tail);

            switch(P.size)
            {
                case 0:
                    return;

                case 1:
                    assert(0!=P.head);
                    assert(0==P.head->next);
                    assert(0==P.head->prev);
                    L.head = L.tail = P.head;
                    break;

                default:
                    assert(P.size>=2);
                    assert(0!=P.head);
                    NODE *prev = L.head = P.head;
                    NODE *node = P.head->next;
                    while(0!=node)
                    {
                        node->prev = prev;
                        prev = node;
                        node = node->next;
                    }
                    assert(0!=prev);
                    L.tail = prev;
                    break;
            }

            //------------------------------------------------------------------
            // update metrics
            //------------------------------------------------------------------
            Coerce(L.size) = P.size;
            Coerce(P.size) = 0;
            P.head = 0;
        };

        //______________________________________________________________________
        //
        //
        //! convert a list into a pool which must be initially empty
        //
        //______________________________________________________________________
        template <typename NODE> static inline
        void ListToPool(PoolOf<NODE> &P, ListOf<NODE> &L) noexcept
        {
            assert(0==P.size);
            assert(0==P.head);

            P.head         = L.head; L.head = L.tail = 0;
            Coerce(P.size) = L.size; Coerce(L.size)  = 0;
            
            for(NODE *node=P.head;node;node=node->next)
                node->prev = 0;
        }

    };

}

#endif

