//! \file

#ifndef Y_Ordered_PrioQ_Included
#define Y_Ordered_PrioQ_Included 1

#include "y/type/args.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{

    namespace Core
    {
        template <typename T>
        struct PrioQ
        {
            Y_ARGS_EXPOSE(T);

            //! insert with ENOUGH linear memory
            /**
             \param tree    linear space
             \param count   insert position (updated)
             \param args    for copy constructor
             \param compare comparison function, test only < 0
             */
            template <typename COMPARE> static inline
            void Insert(MutableType  *tree,
                        size_t       &count,
                        ConstType    &args,
                        COMPARE      &compare)
            {
                assert(tree);

                //------------------------------------------------------------------
                // insert at ipos=count
                //------------------------------------------------------------------
                size_t ipos = count;
                new ( &tree[ipos] ) MutableType(args);
                ++count;

                //------------------------------------------------------------------
                // repetitive promotion (no throw)
                //------------------------------------------------------------------
                while(ipos>0)
                {
                    const size_t  ppos   = (ipos-1)>>1;
                    MutableType  &myself = tree[ipos];
                    MutableType  &parent = tree[ppos];
                    if( Negative == compare(parent,myself) ) {
                        Memory::OutOfReach::Swap(myself,parent);
                        ipos = ppos;
                        continue; // check next promotion from ipos
                    }
                    else
                        break;    // balanced
                }

                //------------------------------------------------------------------
                // done
                //------------------------------------------------------------------
            }


            //! removing top element
            template <typename COMPARE> static inline
            void Remove(MutableType  *tree,
                        size_t       &count,
                        COMPARE      &compare) noexcept
            {
                assert(count>0);

                //------------------------------------------------------------------
                // filter cases
                //------------------------------------------------------------------
                switch(count)
                {
                    case 0: // shouldn't get here..
                        return;

                    case 1: // last item
                        Memory::OutOfReach::Naught( destructed( &tree[0]) );
                        count = 0;
                        return;

                    default:
                        break;
                }
                assert(count>1);

                //------------------------------------------------------------------
                // contract tree: put last item at top
                //------------------------------------------------------------------
                {
                    void *target =  Memory::OutOfReach::Addr( Destructed( &tree[0]) );
                    void *source =  Memory::OutOfReach::Addr( &tree[--count] );
                    Memory::OutOfReach::Grab(target,source,sizeof(Type));
                }

                //------------------------------------------------------------------
                // then rearrange tree
                //------------------------------------------------------------------
                size_t       ipos = 0;
            PROMOTE:
                const size_t temp = ipos<<1;
                const size_t lpos = temp+1;
                const size_t rpos = temp+2;
                size_t mpos = ( lpos<count && Negative == compare(tree[ipos],tree[lpos]) ) ? lpos : ipos;
                if( rpos<count && compare(tree[mpos],tree[rpos])<0 ) mpos = rpos;

                if(mpos==ipos)
                {
                    // done
                    return;
                }
                else
                {
                    // promote
                    mswap(tree[ipos],tree[mpos]);
                    ipos = mpos;
                    goto PROMOTE;
                }

            }

            //! helper to destruct all
            static inline void Finish(MutableType *tree, size_t &count) noexcept
            {
                while(count>0)
                    (void) Memory::OutOfReach::Naught( &tree[--count] );
            }
        };
    }

}

#endif

