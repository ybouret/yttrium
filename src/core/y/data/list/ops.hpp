//! \file

#ifndef Y_Data_List_Ops_Included
#define Y_Data_List_Ops_Included 1

#include "y/config/starting.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{

    struct ListOps
    {

    private:
        template <typename LIST, typename NODE> static inline
        void Starting_(LIST &L, NODE *node) noexcept
        {
            L.head = L.tail = node;
        }

        template <typename LIST, typename NODE> static inline
        void PushTail_(LIST &L, NODE *node) noexcept
        {
            assert(L.size>0);
            assert(0!=L.tail);
            L.tail->next = node;
            node->prev   = L.tail;
            L.tail       = node;
        }

        template <typename LIST, typename NODE> static inline
        void PushHead_(LIST &L, NODE *node) noexcept
        {
            assert(L.size>0);
            assert(0!=L.head);
            L.head->prev = node;
            node->next   = L.head;
            L.head       = node;
        }

        template <typename LIST, typename NODE> static inline
        NODE * Upgraded_(LIST &L, NODE *node) noexcept
        {
            ++Coerce(L.size); assert(L.owns(node));
            return node;
        }

        template <typename NODE> static inline
        void PutAfter_(NODE *mine, NODE *node) noexcept
        {
            assert(0!=mine);
            assert(0!=mine->next);
            NODE *next = mine->next;
            mine->next = node;
            node->prev = mine;
            node->next = next;
            next->prev = node;
        }

    public:
        template <typename LIST, typename NODE> static
        inline NODE * PushTail(LIST &L, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);

            if(L.size<=0)
                Starting_(L,node);
            else
                PushTail_(L,node);

            return Upgraded_(L,node);
        }

        template <typename LIST, typename NODE> static
        inline NODE * PushHead(LIST &L, NODE *node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);

            if(L.size<=0)
                Starting_(L,node);
            else
                PushHead_(L,node);

            return Upgraded_(L,node);
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

        template <typename LIST, typename NODE> static inline
        NODE *InsertAfter(LIST &L, NODE *mine, NODE *node) noexcept
        {
            assert(L.owns(mine));
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            if(0==mine->next)
            {
                assert(mine == L.tail);
                PushTail_(L,node);
            }
            else
            {
                PutAfter_(mine,node);
            }
            return Upgraded_(L,node);
        }

        template <typename LIST, typename NODE, typename COMPARE> static inline
        NODE *InsertOrdered(LIST &L, NODE *node, COMPARE &compare) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);

            switch(L.size)
            {
                case 0: // initialize with first node
                    Starting_(L,node);
                    return Upgraded_(L,node);


                case 1: // special case
                    switch( compare(node,L.head) )
                    {
                        case Negative:
                            // push at head since 'node<L.head'
                            PushHead_(L,node);
                            break;

                        case __Zero__:
                        case Positive:
                            // push at tail since 'node>=L.head'
                            PushTail_(L,node);
                            break;
                    }
                    return Upgraded_(L,node);

                default: // generic case
                    break;
            }

            assert(L.size>=2);

            NODE *lower = L.head;
            switch( compare(node,lower) )
            {
                case Negative:
                    PushHead_(L,node);
                    return Upgraded_(L,node);

                case __Zero__:
                    PutAfter_(lower,node);
                    return Upgraded_(L,node);

                case Positive:
                    break;
            }

            NODE *upper = L.tail;
            switch( compare(node,upper) )
            {
                case Negative:
                    break;

                case __Zero__:
                case Positive:
                    PushTail_(L,node);
                    return Upgraded_(L,node);
            }

        CYCLE:
            NODE *probe = lower->next;
            if(probe!=upper)
            {
                switch( compare(node,probe) )
                {
                    case Negative:
                        PutAfter_(lower,node);
                        return Upgraded_(L,node);

                    case __Zero__:
                        PutAfter_(probe,node);
                        return Upgraded_(L,node);

                    case Positive:
                        break;
                }
                lower = probe;
                goto CYCLE;
            }

            PutAfter_(lower,node);
            return Upgraded_(L,node);
        }

        template <typename NODE> static inline
        SignType IncreasingAddresses(const NODE *lhs, const NODE *rhs) noexcept
        {
            return SignOf(lhs,rhs);
        }


    };

}

#endif

