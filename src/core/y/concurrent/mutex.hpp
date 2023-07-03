
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

        //______________________________________________________________________
        //
        //
        //
        //! Mutex object
        //
        //
        //______________________________________________________________________
        class Mutex : public Lockable
        {
        public:
            explicit Mutex(const char *id); //!< setup with optional id
            virtual ~Mutex() noexcept;      //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mutex);
            Y_LOCKABLE_API();
            Quark::Mutex *mutex;
        };

        
    }
}

#endif
