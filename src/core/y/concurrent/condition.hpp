//! \file

#ifndef Y_Concurrent_Condition_Included
#define Y_Concurrent_Condition_Included 1

#include "y/concurrent/mutex.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        namespace Nucleus
        {
            class Condition;
            class Quark;
        }

        class Condition
        {
        public:
            explicit Condition();
            virtual ~Condition() noexcept;

            void wait(Mutex &) noexcept; //!< wait on a LOCKED mutex
            void signal()      noexcept; //!< signal    one waiting mutex
            void broadcast()   noexcept; //!< broadcast all waiting mutex(es)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Condition);
            Nucleus::Quark     &quark;
            Nucleus::Condition *cond;
        };

    }

}

#endif
