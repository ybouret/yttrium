
#include "y/memory/pages.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/lockable.hpp"
#include "y/memory/out-of-reach.hpp"

#include <cstring>

namespace Yttrium
{

    namespace Memory
    {

        const char * const Pages:: CallSign = "Memory::Pages";

        Pages:: ~Pages() noexcept
        {

            const ScopedLock guard(access);
            while(size)
            {
                --Coerce(delta);
                free( popHead() );
            }

            if(delta!=0)
            {
                std::cerr << "*** " << CallSign << "[" << bytes << "] delta=" << delta << std::endl;
            }
        }

        static unsigned checkShift(const unsigned userShift)
        {
            if(userShift<Pages::MinShift)
                return Pages::MinShift;

            if(userShift>Pages::MaxShift)
                throw Specific::Exception(Pages::CallSign,"2^%u exceeds capacity",userShift);

            return userShift;
        }

        Pages:: Pages(const unsigned userShift) :
        ListOf<Page>(),
        shift( checkShift(userShift) ),
        bytes( Base2<size_t>::One << shift ),
        delta(0),
        access( Lockable::Giant() )
        {
        }

        void * Pages:: acquire() {
            const ScopedLock guard(access);
            void  *page = calloc(1,bytes);
            if(!page) throw Libc::Exception(ENOMEM,"Memory::Pages(%lu)", (unsigned long)bytes);
            ++Coerce(delta);
            return page;
        }

        void Pages:: release(void *page) noexcept
        {
            const ScopedLock guard(access);
            assert(0!=page);
            --Coerce(delta);
            free(page);
        }

        void * Pages:: request()
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

        void Pages:: dismiss(void *addr) noexcept
        {
            assert(0!=addr);
            pushHead(static_cast<Page *>(memset(addr,0,sizeof(Page))));
        }

        void  Pages:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                Page *page = static_cast<Page *>(acquire());
                assert( OutOfReach::Are0(page,sizeof(Page) ) );
                pushTail(page);
            }
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

