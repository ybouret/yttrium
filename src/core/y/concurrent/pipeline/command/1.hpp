
//! \file

#ifndef Y_Concurrent_Command1_Included
#define Y_Concurrent_Command1_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/args/1.hpp"

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
        template <typename OBJECT, typename METHOD, typename ARG1>
        class Command1 : public Runnable, public OneArgument<ARG1>
        {
        public:
            //! setup
            inline explicit Command1(OBJECT                              &o,
                                     METHOD                               m,
                                     typename TypeTraits<ARG1>::ParamType a1) noexcept :
            Runnable(),
            host(o),
            meth(m),
            OneArgument<ARG1>(a1)
            {}

            //! cleanup
            inline virtual ~Command1() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command1);
            inline virtual void run(const ThreadContext &context) { (host.*meth)(context,this->arg1); }

            OBJECT &host;
            METHOD  meth;
        };

    }

}

#endif

