
//! \file

#ifndef Y_Memory_Buffer_Ops_Included
#define Y_Memory_Buffer_Ops_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //! Operations on Buffers
        //
        //______________________________________________________________________
        struct BufferOps
        {

            //! find maximum homology within list of REFERENCE to buffers (Small::LightList)
            template <typename REF_LIST> static inline
            size_t Homology(const REF_LIST &blist) noexcept
            {
                typedef typename REF_LIST::NodeType NODE;
                size_t len=0;
                if(blist.size>=2)
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    assert(0!=blist.head);
                    assert(0!=blist.head->next);
                    len = PairHomology( **(blist.head), **(blist.head->next) );

                    //----------------------------------------------------------
                    // loop do decrease homology
                    //----------------------------------------------------------
                    for(const NODE *lhs=blist.head;lhs;lhs=lhs->next)
                    {
                        for(const NODE *rhs=lhs->next;rhs;rhs=rhs->next)
                        {
                            len = Min(len,PairHomology(**lhs,**rhs));
                        }
                    }
                }

                return len;
            }



        private:
            static size_t PairHomology(const ReadOnlyBuffer &lhs,
                                       const ReadOnlyBuffer &rhs) noexcept;
        };

    }
}

#endif

