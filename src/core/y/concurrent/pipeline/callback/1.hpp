

//! \file

#ifndef Y_Concurrent_Callback1_Included
#define Y_Concurrent_Callback1_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/args/1.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //! function or functionoid, using copy/copy constructor
        //
        //______________________________________________________________________
        template <typename FUNCTION, typename ARG1>
        class Callback1 : public Runnable, public OneArgument<ARG1>
        {
        public:

            //! setup
            inline explicit Callback1(const FUNCTION &fn,
                                      typename TypeTraits<ARG1>::ParamType a1) :
            Runnable(),
            OneArgument<ARG1>(a1),
            function(fn)
            {
            }

            //! cleanup
            inline virtual ~Callback1() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback1);
            inline virtual void run(const ThreadContext &context) { function(context,this->arg1); }

            FUNCTION function;
        };

    }

}

#endif
