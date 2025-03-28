
//! \file


#ifndef Y_Chemical_Restartable_Included
#define Y_Chemical_Restartable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Restartable
        {
        public:
            static const unsigned     Width = 22;
            static const char * const None;

        protected:
            explicit Restartable() noexcept;

        public:
            virtual ~Restartable() noexcept;

            virtual void restart() noexcept = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Restartable);
        };

    }

}

#endif

