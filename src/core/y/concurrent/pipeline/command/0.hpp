//! \file

#ifndef Y_Concurrent_Command0_Included
#define Y_Concurrent_Command0_Included 1

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
        class Command0 : public Runnable
        {
        public:
            inline explicit Command0(OBJECT &o, METHOD m) noexcept : Runnable(), host(o), meth(m) {} //!< setup
            inline virtual ~Command0() noexcept {}                                                   //!< cleanup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command0);
            inline virtual void run(const ThreadContext &context) { (host.*meth)(context); }

            OBJECT &host;
            METHOD  meth;
        };

    }

}

#endif

