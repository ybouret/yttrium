


//! \file

#ifndef Y_Concurrent_Command_Included
#define Y_Concurrent_Command_Included 1

#include "y/concurrent/pipeline/runnable.hpp"

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
        template <typename OBJECT, typename METHOD>
        class Command : public Runnable
        {
        public:
            inline explicit Command(OBJECT &o, METHOD m) noexcept : Runnable(), host(o), meth(m) {} //!< setup
            inline virtual ~Command() noexcept {}                                                   //!< cleanup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command);
            inline virtual void run(const ThreadContext &context) { (host.*meth)(context); }

            OBJECT &host;
            METHOD  meth;
        };

    }

}

#endif

