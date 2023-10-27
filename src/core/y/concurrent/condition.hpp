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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Condition);
            Nucleus::Quark     &quark;
            Nucleus::Condition *cond;
        };

    }

}

#endif
