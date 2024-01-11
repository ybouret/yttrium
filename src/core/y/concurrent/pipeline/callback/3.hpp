



//! \file

#ifndef Y_Concurrent_Callback3_Included
#define Y_Concurrent_Callback3_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/args/3.hpp"

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
        template <typename FUNCTION, typename ARG1, typename ARG2, typename ARG3>
        class Callback3 : public Runnable, public ThreeArguments<ARG1,ARG2,ARG3>
        {
        public:

            //! setup
            inline explicit Callback3(const FUNCTION                      &fn,
                                      typename TypeTraits<ARG1>::ParamType a1,
                                      typename TypeTraits<ARG2>::ParamType a2,
                                      typename TypeTraits<ARG3>::ParamType a3) :
            Runnable(),
            ThreeArguments<ARG1,ARG2,ARG3>(a1,a2,a3),
            function(fn)
            {
            }

            //! cleanup
            inline virtual ~Callback3() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback3);
            inline virtual void run(const ThreadContext &context) { function(context,this->arg1,this->arg2,this->arg3); }

            FUNCTION function;
        };

    }

}

#endif
