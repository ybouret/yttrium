//! \file

#ifndef Y_Data_List_Ops_Included
#define Y_Data_List_Ops_Included 1

#include "y/config/starting.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{


    //__________________________________________________________________________
    //
    //
    //
    //! Operations on List
    //
    //
    //__________________________________________________________________________
    struct ListOps
    {

    private:
        //______________________________________________________________________
        //
        //! start list with node, do not change size
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        void Starting_(LIST &L, NODE * const node) noexcept
        {
            L.head = L.tail = node;
        }

        //______________________________________________________________________
        //
        //! push node at tail, do not change size
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        void PushTail_(LIST &L, NODE * const node) noexcept
        {
            assert(L.size>0);
            assert(0!=L.tail);
            L.tail->next = node;
            node->prev   = L.tail;
            L.tail       = node;
        }

        //______________________________________________________________________
        //
        //! push node at head, do not change size
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        void PushHead_(LIST &L, NODE * const node) noexcept
        {
            assert(L.size>0);
            assert(0!=L.head);

            L.head->prev = node;
            node->next   = L.head;
            L.head       = node;
        }

        //______________________________________________________________________
        //
        //! increase size and return node
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE * Upgraded_(LIST &L, NODE * const node) noexcept
        {
            ++Coerce(L.size); assert(L.owns(node));
            return node;
        }

        //______________________________________________________________________
        //
        //! increase size and return node
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE * Released_(LIST &L, NODE * const node) noexcept
        {
            --Coerce(L.size); assert(!L.owns(node));
            return node;
        }

        //______________________________________________________________________
        //
        //! put after a regular node
        //______________________________________________________________________
        template <typename NODE> static inline
        void PutAfter_(NODE * const mine, NODE * const node) noexcept
        {
            assert(0!=mine);
            assert(0!=mine->next);
            NODE *next = mine->next;
            mine->next = node;
            node->prev = mine;
            node->next = next;
            next->prev = node;
        }

        //______________________________________________________________________
        //
        //! put before a regular node
        //______________________________________________________________________
        template <typename NODE> static inline
        void PutBefore_(NODE * const mine, NODE * const node) noexcept
        {
            assert(0!=mine);
            assert(0!=mine->prev);
            NODE *prev = mine->prev;
            node->next = mine;
            node->prev = prev;
            prev->next = node;
            mine->prev = node;
        }

    public:

        //______________________________________________________________________
        //
        //! check node is owned by list
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        bool OwnedBy(const LIST &L, const NODE * const node) noexcept
        {
            for(const NODE *scan=L.head;scan;scan=scan->next)
            {
                if(scan==node) return true;
            }
            return false;
        }

        //______________________________________________________________________
        //
        //! push node at tail of list
        //______________________________________________________________________
        template <typename LIST, typename NODE> static
        inline NODE * PushTail(LIST &L, NODE *const node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            assert(!OwnedBy(L,node));

            if(L.size<=0)
                Starting_(L,node);
            else
                PushTail_(L,node);

            return Upgraded_(L,node);
        }

        //______________________________________________________________________
        //
        //! push node at head of list
        //______________________________________________________________________
        template <typename LIST, typename NODE> static
        inline NODE * PushHead(LIST &L, NODE * const node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            assert(!OwnedBy(L,node));

            if(L.size<=0)
                Starting_(L,node);
            else
                PushHead_(L,node);

            return Upgraded_(L,node);
        }


        //______________________________________________________________________
        //
        //! pop tail node
        //______________________________________________________________________
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
            return Released_(L,node);
        }

        //______________________________________________________________________
        //
        //! pop head node
        //______________________________________________________________________
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
            return Released_(L,node);
        }

        //______________________________________________________________________
        //
        //! pop any node
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE *Pop(LIST &L, NODE * const node) noexcept
        {
            assert(OwnedBy(L,node));
            if(L.head==node)
                return PopHead(L);
            else
            {
                if(L.tail==node)
                    return PopTail(L);
                else
                {
                    assert(L.size>2);
                    assert(L.head!=node);
                    assert(L.tail!=node);
                    NODE *next = node->next;
                    NODE *prev = node->prev;
                    next->prev = prev;
                    prev->next = next;
                    node->next = 0;
                    node->prev = 0;
                    return Released_(L,node);
                }
            }
        }

        //______________________________________________________________________
        //
        //! insert node after L.mine
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE *InsertAfter(LIST &L, NODE *const mine, NODE * const node) noexcept
        {
            assert(OwnedBy(L,mine));
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

        //______________________________________________________________________
        //
        //! insert node before L.mine
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE *InsertBefore(LIST &L, NODE * const mine, NODE * const node) noexcept
        {
            assert(OwnedBy(L,mine));
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            if(0==mine->prev)
            {
                assert(mine == L.head);
                PushHead_(L,node);
            }
            else
            {
                PutBefore_(mine,node);
            }
            return Upgraded_(L,node);
        }




        //______________________________________________________________________
        //
        //! target = target+source, source=0
        //______________________________________________________________________
        template <typename TARGET, typename SOURCE> static inline
        void MergeTail(TARGET &target, SOURCE &source) noexcept
        {
            if(source.size<=0) return;

            if(target.size<=0)
            {
                Swap(target.head,source.head);
                Swap(target.tail,source.tail);
                CoerceSwap(target.size,source.size);
            }
            else
            {
                assert(0!=target.tail);
                assert(0!=source.head);
                target.tail->next    = source.head;
                source.head->prev    = target.tail;
                target.tail          = source.tail;
                Coerce(target.size) += source.size;
                source.head          = 0;
                source.tail          = 0;
                Coerce(source.size)  = 0;
            }
        }


        

        //______________________________________________________________________
        //
        //! move node to next count, may return NULL=last
        //______________________________________________________________________
        template <typename NODE> static inline
        NODE *Next(NODE *node, size_t count) noexcept
        {
            while(count-- > 0)
            {
                assert(0!=node);
                node = node->next;
            }
            return node;
        }

        //______________________________________________________________________
        //
        //! move node to prev count, may return NULL=last
        //______________________________________________________________________
        template <typename NODE> static inline
        NODE *Prev(NODE *node, size_t count) noexcept
        {
            while(count-- > 0)
            {
                assert(0!=node);
                node = node->prev;
            }
            return node;
        }

        //______________________________________________________________________
        //
        //! access node in [1..size]
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE *Fetch(LIST &L, size_t indx) noexcept
        {
            assert(1<=indx); assert(indx<=L.size);
            const size_t size = L.size;
            const size_t half = size >> 1;
            if(indx<=half)
            {
                return Next(L.head,--indx);
            }
            else
            {
                return Prev(L.tail,size - indx);
            }
        }




        //______________________________________________________________________
        //
        //!  lhs=src/2, rhs=src-lhs
        //______________________________________________________________________
        template <typename LIST, typename SOURCE> static inline
        void Divide(LIST &lhs, LIST &rhs, SOURCE &src) noexcept
        {
            typedef typename LIST::NodeType NodeType;

            // sanity check
            assert(src.size>=2);
            assert(0==lhs.size);
            assert(0==rhs.size);

            // compute half size and get half node
            const size_t size = src.size;
            const size_t half = size >> 1; assert(half>0);
            NodeType *   head = src.head;  assert(0!=head);
            NodeType    *node = Next(head,half-1);

            // create lhs sub-list
            lhs.head         = head;
            lhs.tail         = node;
            Coerce(lhs.size) = half;

            // create rhs sub-list
            rhs.head         = node->next;   assert(0!=node->next);
            rhs.tail         = src.tail;
            Coerce(rhs.size) = size-half;

            // detach lists
            rhs.head->prev = 0;
            lhs.tail->next = 0;

            // cleanup src
            src.head = 0;
            src.tail = 0;
            Coerce(src.size) = 0;

        }

        //______________________________________________________________________
        //
        //! fusion of two sorted list into another sorted
        //______________________________________________________________________
        template <typename TARGET, typename LIST, typename COMPARE_NODES> static inline
        void Fusion(TARGET &target, LIST &lhs, LIST &rhs, COMPARE_NODES &compareNodes) noexcept
        {
            assert(0==target.size);
            assert(0==target.head);
            assert(0==target.tail);
            while(lhs.size>0 && rhs.size>0)
            {
                switch( compareNodes(lhs.head, rhs.head) )
                {
                    case Negative:
                    case __Zero__:
                        PushTail(target, PopHead(lhs) ); break;

                    case Positive:
                        PushTail(target, PopHead(rhs) ); break;
                }
            }
            assert(0==lhs.size||0==rhs.size);
            MergeTail(target,lhs);
            MergeTail(target,rhs);
        }


        //______________________________________________________________________
        //
        //! generic insertion of a node into a sorted list
        /**
         \param L list
         \param node a valid node
         \param compare SignType compare(lhsNode,rhsNode)
         */
        //______________________________________________________________________
        template <typename LIST, typename NODE, typename COMPARE> static inline
        NODE *InsertOrdered(LIST &L, NODE * const node, COMPARE &compare) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            assert(!OwnedBy(L,node));

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

        //______________________________________________________________________
        //
        //! helper to test InsertOrdered
        //______________________________________________________________________
        template <typename NODE> static inline
        SignType IncreasingAddresses(const NODE * const lhs, const NODE * const rhs) noexcept
        {
            return Sign::Of(lhs,rhs);
        }

        //______________________________________________________________________
        //
        //! helper to test insertion results
        //______________________________________________________________________
        template <typename LIST> static inline
        bool CheckIncreasingAddresses(const LIST &L) noexcept
        {
            for(const typename LIST::NodeType *node=L.head;node;node=node->next)
            {
                if(0!=node->next && node->next < node) return false;
            }
            return true;
        }

        //______________________________________________________________________
        //
        //! specific optimized insertion by increasing address
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE *InsertByIncreasingAddress(LIST &L, NODE * const node) noexcept
        {
            assert(0!=node);
            assert(0==node->next);
            assert(0==node->prev);
            assert(!OwnedBy(L,node));

            switch(L.size)
            {
                case 0:
                    Starting_(L,node);
                    return Upgraded_(L,node);

                case 1:
                    if(node<L.head)
                    {
                        PushHead_(L,node);
                    }
                    else
                    {
                        assert(L.head<node);
                        PushTail_(L,node);
                    }
                    return Upgraded_(L,node);

                default:
                    break;

            }
            assert(L.size>=2);
            NODE *lower = L.head;
            if(node<lower)
            {
                PushHead_(L,node);
                return Upgraded_(L,node);
            }

            assert(node>lower);
            NODE *upper = L.tail;
            if(node>upper)
            {
                PushTail_(L,node);
                return Upgraded_(L,node);
            }

            {
                NODE *probe = lower->next; assert(probe);
                while(probe!=upper)
                {
                    if(node<probe)
                        break;
                    lower = probe;
                    probe = lower->next;
                }
                upper = probe;
            }
            assert(node>lower);
            assert(node<upper);
            return InsertAfter(L,lower,node);
        }

        //______________________________________________________________________
        //
        //! move node to front
        //______________________________________________________________________
        template <typename LIST, typename NODE> static inline
        NODE *MoveToFront(LIST &L, NODE * const node) noexcept
        {
            assert(OwnedBy(L,node));
            assert(L.size>0);

            //------------------------------------------------------------------
            // trivial cases
            //------------------------------------------------------------------
            switch(L.size)
            {
                case 0: // N/A
                case 1:
                    return node;

                case 2:
                    if(node==L.tail)
                    {
                        Swap(L.head,L.tail);
                        L.head->prev = 0;
                        L.head->next = L.tail;
                        L.tail->prev = L.head;
                        L.tail->next = 0;
                    } else { assert(node==L.head); }
                    return node;

                default:
                    break;
            }

            if(L.head==node) {
                return node;
            }
            else
            {
                //--------------------------------------------------------------
                // detach node
                //--------------------------------------------------------------
                if(L.tail==node)
                {
                    assert(0!=L.tail->prev);
                    assert(0==L.tail->next);

                    L.tail = L.tail->prev;
                    L.tail->next = 0;
                    node->prev = 0;
                }
                else
                {
                    NODE *prev = node->prev; assert(0!=prev);
                    NODE *next = node->next; assert(0!=next);
                    prev->next = next;
                    next->prev = prev;
                    node->prev = 0;
                }

                //--------------------------------------------------------------
                // push at head
                //--------------------------------------------------------------
                PushHead_(L,node); assert(L.head==node);
                return node;
            }
        }

        template <typename LIST, typename NODE> static inline
        NODE *TowardsHead(LIST &L, NODE * const node) noexcept
        {
            assert(0!=node);
            assert(L.owns(node));
            assert(L.size>=2);
            assert(L.head!=node);
            assert(0!=node->prev);
            return InsertBefore(L,node->prev,Pop(L,node));
        }
    };

}

#endif

