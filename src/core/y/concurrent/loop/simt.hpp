
//! \file


#ifndef Y_Concurrent_SIMT_Included
#define Y_Concurrent_SIMT_Included 1

#include "y/concurrent/loop/simt/loop.hpp"
#include "y/concurrent/frames.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Frames+Loop : Single Instuction Multiple Threads
        //
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class SIMT : public Frames<ENGINE>, public SIMT_Loop
        {
        public:
            //! setup
            inline explicit  SIMT( const SharedLoop &sl ) :
            Frames<ENGINE>(sl),
            SIMT_Loop(sl)
            {
            }

            //! cleanup
            inline virtual ~SIMT() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT);
        };
    }

}

#endif
