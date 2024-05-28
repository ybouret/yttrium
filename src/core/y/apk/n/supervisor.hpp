
//! \file

#ifndef Y_APK_Natural_Supervisor_Included
#define Y_APK_Natural_Supervisor_Included 1

#include "y/singleton.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    namespace APK
    {
        class Latch : public Object, public Concurrent::Mutex
        {
        public:
            explicit Latch() noexcept;
            virtual ~Latch() noexcept;
            Latch * next;
            Latch * prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Latch);
        };

        class Supervisor : public Singleton<Supervisor>
        {
        public:
            static const char * const      CallSign; //!< "APK::Supervisoir"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 1000;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Supervisor);
            explicit Supervisor() noexcept;
            virtual ~Supervisor() noexcept;
            friend class Singleton<Supervisor>;
        };
    }

}

#endif

