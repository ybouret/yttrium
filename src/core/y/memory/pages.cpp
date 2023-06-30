
#include "y/memory/pages.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"


namespace Yttrium
{

    namespace Memory
    {

        Pages:: ~Pages() noexcept
        {

        }

        static size_t checkShift(const size_t userShift)
        {
            if(userShift<Pages::MinShift)
                return Pages::MinShift;

            if(userShift>Pages::MaxShift)
                throw Specific::Exception("Memory::Pages","2^%lu exceeds capacity",(unsigned long)userShift);

            return userShift;
        }

        Pages:: Pages(const size_t userShift) :
        ListOf<Page>(),
        shift( checkShift(userShift) ),
        bytes( Base2<size_t>::One << shift ),
        access( Lockable::Giant() )
        {
        }

        void * Pages:: acquire() {
            const ScopedLock guard(access);
            void  *page = calloc(1,bytes);
            if(!page) throw Libc::Exception(ENOMEM,"Pages(%lu)", (unsigned long)bytes);
            return page;
        }

        void Pages:: release(void *page) noexcept
        {
            const ScopedLock guard(access);
            assert(0!=page);
            free(page);
        }

        void * Pages:: query()
        {
            if(size>0)
            {
                return popHead();
            }
            else
            {
                return acquire();
            }
        }

        void Pages:: store(void *addr) noexcept
        {
            assert(0!=addr);

        }


    }

}

#if defined(Y_BSD)
#include <unistd.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Yttrium
{

    namespace Memory
    {


        size_t Page:: QueryOsBytes() noexcept
        {

#if defined(Y_BSD)
            long sz = sysconf(_SC_PAGESIZE);
            if(sz<=0) return DefaultBytes;
            return sz;
#endif

#if defined(Y_WIN)
            SYSTEM_INFO si;
            ::GetSystemInfo(&si);
            if(si.dwPageSize<=0) return DefaultBytes;
            return si.dwPageSize;
#endif

        }
    }
}

