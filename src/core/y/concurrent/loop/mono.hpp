
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
            static const char * const CallSign;

            explicit Mono(const char *id=0) noexcept;
            virtual ~Mono() noexcept;

            virtual const char * callSign() const noexcept;
            virtual size_t       size()     const noexcept;
            virtual ConstType & operator[](const size_t) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mono);

            FakeLock      access;
            ThreadContext context;
        };

    }

}

#endif
