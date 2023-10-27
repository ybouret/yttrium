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
        }

        //______________________________________________________________________
        //
        //
        //
        //! Condition variable
        //
        //
        //______________________________________________________________________
        class Condition : public Primitive
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Condition();
            virtual ~Condition() noexcept;

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________
            void wait(Mutex &) noexcept; //!< wait on a LOCKED mutex, that will return unlocked
            void signal()      noexcept; //!< signal    one waiting mutex
            void broadcast()   noexcept; //!< broadcast all waiting mutex(es)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Condition);
            Nucleus::Condition *cond;
        };

    }

}

#endif
