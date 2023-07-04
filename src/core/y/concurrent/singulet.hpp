//! \file

#ifndef Y_Concurrent_Singulet_Included
#define Y_Concurrent_Singulet_Included 1

#include "y/concurrent/mutex.hpp"
#include "y/system/at-exit.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class Singulet
        {
        protected:
            explicit Singulet() noexcept;

        public:
            virtual ~Singulet() noexcept;

            virtual const char *      callSign() const noexcept = 0;
            virtual AtExit::Longevity lifeTime() const noexcept = 0;

            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Singulet);
        };
    }
}

#endif

