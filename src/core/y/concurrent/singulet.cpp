
#include "y/concurrent/singulet.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Concurrent
    {

        bool Singulet:: Verbose = false;

        Singulet:: ~Singulet() noexcept
        {
        }

        Singulet:: Singulet() noexcept
        {
        }


        void Singulet:: CheckLifeTime(const char             *whose,
                                      const AtExit::Longevity which)
        {
            if(which>=AtExit::MaximumLongevity)
            {
                if(!whose) whose = Core::Unknown;
                throw Specific::Exception(whose,"LifeTime cannot be Maxium Longevity!");
            }
        }

        void Singulet:: CheckInstance(const char *whose,
                                      void       *where)
        {
            if(!where)
            {
                if(!whose) whose = Core::Unknown;
                Libc::CriticalError(EINVAL, "No %s::Instance !!", whose);
            }
        }
    }


}


