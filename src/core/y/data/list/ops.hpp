//! \file

#ifndef Y_Data_List_Ops_Included
#define Y_Data_List_Ops_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    struct ListOps
    {
        template <typename LIST, typename NODE> static
        inline NODE * PushTail(LIST &L, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);

            if(L.size<=0) {
                L.head = L.tail = node;
            }
            else {
                L.tail->next = node;
                node->prev   = L.tail;
                L.tail       = node;
            }
            ++Coerce(L.size);
            return node;
        }

        template <typename LIST, typename NODE> static
        inline NODE * PushHead(LIST &L, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);

            if(L.size<=0) {
                L.head = L.tail = node;
            }
            else {
                L.head->prev = node;
                node->next   = L.head;
                L.head       = node;
            }
            ++Coerce(L.size);
            return node;
        }


        template <typename LIST> static
        inline typename LIST::NodeType * PopTail(LIST &L) noexcept
        {
            // sanity check
            assert(L.size>0);
            assert(0!=L.head);
            assert(0!=L.tail);

            typename LIST::NodeType *node = L.tail;
            if(1==L.size)
            {
                assert(L.head==L.tail);
                L.head = L.tail = 0;
            }
            else
            {
                assert(L.size>1);
                assert(L.head!=L.tail);
                assert(0!=L.tail->prev);

                L.tail = L.tail->prev;
                L.tail->next = 0;
                node->prev   = 0; assert(0==node->next);
            }
            --Coerce(L.size);
            return node;
        }

        template <typename LIST> static
        inline typename LIST::NodeType * PopHead(LIST &L) noexcept
        {
            // sanity check
            assert(L.size>0);
            assert(0!=L.head);
            assert(0!=L.tail);

            typename LIST::NodeType *node = L.head;
            if(1==L.size)
            {
                assert(L.head==L.tail);
                L.head = L.tail = 0;
            }
            else
            {
                assert(L.size>1);
                assert(L.head!=L.tail);
                assert(0!=L.head->next);

                L.head = L.head->next;
                L.head->prev = 0;
                node->next   = 0; assert(0==node->prev);
            }
            --Coerce(L.size);
            return node;
        }


    };

}

#endif

