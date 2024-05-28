
#include "y/apk/natural/supervisor.hpp"

namespace Yttrium
{
    namespace APK
    {
        const char * const Supervisor:: CallSign = "APK::Supervisor";


        

        Supervisor:: ~Supervisor() noexcept
        {
        }

        Supervisor:: Supervisor() noexcept : pool()
        {
        }

        Latch * Supervisor:: queryLocked()
        {
            Y_LOCK(access);
            return pool.size > 0 ? pool.query() : new Latch();
        }

        void Supervisor:: storeLocked(Latch *latch) noexcept
        {
            assert(0!=latch);
            Y_LOCK(access);
            pool.store(latch);
        }

        Latch * Latch:: Query()
        {
            static Supervisor &mgr = Supervisor::Instance();
            return mgr.queryLocked();
        }

        void Latch:: Store(Latch *latch) noexcept
        {
            static Supervisor &mgr = Supervisor::Location();
            mgr.storeLocked(latch);
        }


        
    }

}
