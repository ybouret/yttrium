//! \file

#ifndef Y_Concurrent_Loop_Included
#define Y_Concurrent_Loop_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/container/readable.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef Functor<void,TL1(const ThreadContext&)> Kernel;

        class Loop : public Readable<const ThreadContext>
        {
        public:
            virtual ~Loop() noexcept;


        protected:
            explicit Loop() noexcept;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Loop);
        };

    }

}

#endif

