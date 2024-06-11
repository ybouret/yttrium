#include "y/net/api.hpp"

#if defined(Y_WIN)
#include <winsock2.h>
#endif

namespace Yttrium
{

    namespace Network
    {

#if defined(Y_WIN)
        
#endif

        API:: API() : Singleton<API>()
        {
        }

        API:: ~API() noexcept {}
    }

}


