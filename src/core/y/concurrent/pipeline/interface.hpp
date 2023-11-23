//! \file

#ifndef Y_Concurrent_Interface_Included
#define Y_Concurrent_Interface_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/container/readable.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef uint32_t                                  JobID;
        typedef Functor<void,TL1(const ThreadContext &)> Job;

        

        //______________________________________________________________________
        //
        //
        //
        //! Interface to a Pipeline to process jobs
        //
        //
        //______________________________________________________________________
        class Pipeline : public Readable<const ThreadContext>
        {
        public:
            virtual ~Pipeline() noexcept; //!< cleanup




        protected:
            explicit Pipeline() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);
        };

    }

}

#endif

