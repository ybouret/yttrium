#include "y/lockable.hpp"
#include "y/type/utils.hpp"

#include <cstdio>
#include <cstring>

#include <iostream>

namespace Yttrium
{
    
    Lockable:: ~Lockable() noexcept
    {
        memset( (void *)&uuid[0],0, sizeof(uuid) );
    }

    Lockable:: Lockable(const char *id) noexcept :
    depth(0),
    uuid()
    {
        static const size_t MaxLen = sizeof(uuid)-1;

        
        char *buff = (char *)&uuid[0];
        memset(buff,0,sizeof(uuid));

        if(id)
        {
            memcpy(buff,id,Min<size_t>(strlen(id),MaxLen));
        }
        else
        {
            typedef void       *addrType;
            static const size_t addrSize = sizeof(addrType);
            static const size_t hexaSize = 2*addrSize;
            union {
                addrType addr;
                uint8_t  data[addrSize];
            } alias = { this };
            char hexa[hexaSize] = { 0 };
            memset(hexa,0,sizeof(hexa));
            for(size_t i=0,j=0;i<addrSize;++i)
            {
                char temp[4] = { 0,0,0,0 };
                snprintf(temp,4, "%02X", alias.data[i]);
                hexa[j++] = temp[0];
                hexa[j++] = temp[1];
            }

            memcpy(buff,hexa,Min<size_t>(hexaSize,MaxLen));
        }
    }


    void Lockable:: lock()    noexcept
    {
        doLock();
        ++Coerce(depth);
    }

    void Lockable:: unlock() noexcept
    {
        --Coerce(depth);
        doUnlock();
    }

    bool Lockable:: tryLock() noexcept
    {
        if(doTryLock())
        {
            ++Coerce(depth);
            return true;
        }
        else
            return false;
    }

}



namespace Yttrium
{
    ScopedLock:: ScopedLock(Lockable &l) noexcept : host(l)
    {
        host.lock();
    }

    ScopedLock:: ~ScopedLock() noexcept
    {
        host.unlock();
    }
}
