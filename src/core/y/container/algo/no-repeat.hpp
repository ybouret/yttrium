

//! \file

#ifndef Y_Container_Algo_No_Repeat_Included
#define Y_Container_Algo_No_Repeat_Included 1

#include "y/sequence/interface.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Algo
    {
        //______________________________________________________________________
        //
        //
        //! Trim last bad items
        //
        //______________________________________________________________________
        template <typename SEQUENCE, typename IS_BAD>
        inline SEQUENCE &NoRepeat(SEQUENCE &seq, IS_BAD &is_bad)
        {
            size_t i=1;
            while(i<seq.size())
            {
                size_t j = i+1;
                if(is_bad(seq[i])&&is_bad(seq[j]))
                {
                    const size_t n = seq.size();
                    while(j<n)
                    {
                        const size_t k=j+1;
                        Memory::OutOfReach::Swap(seq[j],seq[k]);
                        j=k;
                    }
                    seq.popTail();
                }
                else
                {
                    ++i;
                }
            }

            return seq;
        }
    }

}
#endif

