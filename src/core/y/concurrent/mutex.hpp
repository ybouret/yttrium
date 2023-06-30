
//! \file

#ifndef Yttrium_Concurrent_Fake_Lock_Included
#define Yttrium_Concurrent_Fake_Lock_Included 1


#include "y/lockable.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Quark
        {
            class Mutex;
        }

        class Mutex : public Lockable
        {
        public:
            explicit Mutex(const char *id);
            virtual ~Mutex() noexcept;

            static Lockable &Giant();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mutex);
            Y_LOCKABLE_API();
            Quark::Mutex *mutex;
        };

        
    }
}

#endif
