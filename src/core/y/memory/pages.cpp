
#include "y/memory/pages.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"


#include "y/lockable.hpp"
#include "y/type/utils.hpp"
#include "y/text/human-readable.hpp"

#include <cstring>
#include <iomanip>

namespace Yttrium
{

    namespace Memory
    {

        const char * const Pages:: CallSign = "Memory::Pages";

        Pages:: ~Pages() noexcept
        {
            Y_LOCK(giant);
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


        Pages:: Pages(const unsigned userShift,
                      Lockable      &userMutex) noexcept :
        ListOf<Page>(),
        shift( Max(MinShift,userShift)     ),
        bytes( Base2<size_t>::One << shift ),
        delta(0),
        giant( userMutex )
        {
            assert(shift<=MaxShift);
        }



        void * Pages:: acquire()
        {
            Y_LOCK(giant);
            void  *page = calloc(1,bytes);

            if(!page)
                throw Libc::Exception(ENOMEM,"Memory::Pages(%lu)", (unsigned long)bytes);

            ++Coerce(delta);
            return page;
        }

        void Pages:: release(void *page) noexcept
        {
            const ScopedLock guard(giant);
            assert(0!=page);
            --Coerce(delta);
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
            insertByIncreasingAddress( static_cast<Page *>(memset(addr,0,sizeof(Page))) );
            assert( ListOps:: CheckIncreasingAddresses(*this) );
        }

        void  Pages:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                Page *page = static_cast<Page *>(acquire());
                assert( OutOfReach::Are0(page,sizeof(Page) ) );
                store(page);
            }
        }

        uint64_t Pages:: displayInfo(const size_t indent) const
        {
            const size_t allocated = delta * bytes;
            if(delta>0)
            {
                Core::Indent(std::cerr,indent) << "[" << std::setw(6) << bytes << "] ready = " << std::setw(6) << size << " / " << std::setw(6) << delta << " @" << HumanReadable(allocated) <<std::endl;
            }
            return allocated;
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

