//! \file

#ifndef Y_Concurrent_Split_Included
#define Y_Concurrent_Split_Included 1

#include "y/concurrent/context.hpp"

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
            /**
             \param size number of worker
             \param rank in [0:size-1]
             \param length input: total length. output: worker length
             \param offset input: global offset. output: local offset

             a zero length means nothing to locally do, and offset is reset
             */
            template <typename T, typename U> static inline
            void With(const T size, const T rank, U &length, U &offset) noexcept
            {
                assert(size>0);
                assert(rank<size);
                const U origin = offset;
                T left = size;
                U todo = length/left;
                for(T r=0;r<rank;++r)
                {
                    --left;
                    length -= todo;
                    offset += todo;
                    todo    = length/left;
                }
                if( (length = todo) <= 0 ) offset = origin;
            }

            //! split for a given context
            template <typename U> static inline
            void For(const Context &ctx, U &length, U &offset) noexcept
            {
                With(ctx.size,ctx.rank,length,offset);
            }


        };

    }

}

#endif

