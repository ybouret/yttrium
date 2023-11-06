//! \file

#ifndef Y_Concurrent_Split_Included
#define Y_Concurrent_Split_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //! Split linear task
        //
        //______________________________________________________________________
        struct Split
        {
            //! from total length and offset, return parameters for size.rank
            template <typename T, typename U> static inline
            void With(const T size, const T rank, U &length, U &offset) noexcept
            {
                assert(size>0);
                assert(rank<size);

                T left = size;
                U todo = length/left;
                for(T r=0;r<rank;++r)
                {
                    --left;
                    length -= todo;
                    offset += todo;
                    todo    = length/left;
                }
                length = todo;
            }
            
            
        };

    }

}

#endif

