
//! \file

#ifndef Y_APK_Natural_Supervisor_Included
#define Y_APK_Natural_Supervisor_Included 1

#include "y/apk/natural/latch.hpp"
#include "y/singleton.hpp"

namespace Yttrium
{
    namespace APK
    {
      

        class Supervisor : public Singleton<Supervisor>
        {
        public:
            static const char * const      CallSign; //!< "APK::Supervisoir"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 1000;


            Latch * queryLocked();
            void    storeLocked(Latch *) noexcept;


            Latch::Pool pool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Supervisor);
            explicit Supervisor() noexcept;
            virtual ~Supervisor() noexcept;
            friend class Singleton<Supervisor>;

        };
    }

}

#endif

