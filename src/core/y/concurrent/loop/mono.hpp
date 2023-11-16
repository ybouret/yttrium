
//! \file

#ifndef Y_Concurrent_Mono_Included
#define Y_Concurrent_Mono_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/fake-lock.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class Mono : public Loop
        {
        public:
            explicit Mono(const char *id=0) noexcept;
            virtual ~Mono() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mono);
            FakeLock      access;
            ThreadContext context;
        };

    }

}

#endif
