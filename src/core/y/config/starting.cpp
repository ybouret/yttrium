#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Core
    {
        const char Unknown[] = "unknown";
        const char Success[] = "success";
        const char Failure[] = "failure";
        const char Warning[] = "warning";
        const char Message[] = "message";
        
    }

    bool Die(const char *msg)
    {
        (void)msg;
        return true;
    }


    bool Say(const char *msg)
    {
        (void)msg;
        return false;
    }

    bool Good(const void *addr, const size_t size)
    {
        if(0!=addr)
        {
            return true;
        }
        else
        {
            assert(0==addr);
            return size<=0;
        }

    }
    
}
