


//! \file

#ifndef Y_Concurrent_Callback2_Included
#define Y_Concurrent_Callback2_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/args/2.hpp"

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
        template <typename FUNCTION, typename ARG1, typename ARG2>
        class Callback2 : public Runnable, public TwoArguments<ARG1,ARG2>
        {
        public:

            //! setup
            inline explicit Callback2(const FUNCTION                      &fn,
                                      typename TypeTraits<ARG1>::ParamType a1,
                                      typename TypeTraits<ARG2>::ParamType a2) :
            Runnable(),
            TwoArguments<ARG1,ARG2>(a1,a2),
            function(fn)
            {
            }

            //! cleanup
            inline virtual ~Callback2() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback2);
            inline virtual void run(const ThreadContext &context) { function(context,this->arg1,this->arg2); }

            FUNCTION function;
        };

    }

}

#endif
