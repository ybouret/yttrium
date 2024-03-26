//! \file

#ifndef Y_Concurrent_Pipeline_Multiplex_Included
#define Y_Concurrent_Pipeline_Multiplex_Included 1

#include "y/concurrent/pipeline/multiplex/q.hpp"
#include "y/concurrent/frames.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //! Engine Should derive from Frame0D
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class Multiplex : public Frames<ENGINE>, public MultiplexQueue
        {
        public:
            typedef Writable<ENGINE> Engines;

            inline explicit Multiplex(const SharedPipeline &sp) :
            Frames<ENGINE>(sp),
            MultiplexQueue(sp)
            {
            }

            inline virtual ~Multiplex() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Multiplex);
        };
    }

}


#endif

