

//! \file

#ifndef Y_Concurrent_Command2_Included
#define Y_Concurrent_Command2_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/args/2.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //! encapsulate call to OBJECT.METHOD call
        //
        //______________________________________________________________________
        template <typename OBJECT, typename METHOD, typename ARG1, typename ARG2>
        class Command2 : public Runnable, public TwoArguments<ARG1,ARG2>
        {
        public:
            //! setup
            inline explicit Command2(OBJECT                              &o,
                                     METHOD                               m,
                                     typename TypeTraits<ARG1>::ParamType a1,
                                     typename TypeTraits<ARG2>::ParamType a2) noexcept :
            Runnable(),
            TwoArguments<ARG1,ARG2>(a1,a2),
            host(o),
            meth(m)
            {}

            //! cleanup
            inline virtual ~Command2() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command2);
            inline virtual void run(const ThreadContext &context) { (host.*meth)(context,this->arg1,this->arg2); }

            OBJECT &host;
            METHOD  meth;
        };

    }

}

#endif

