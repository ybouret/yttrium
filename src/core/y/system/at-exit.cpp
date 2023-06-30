#include "y/system/at-exit.hpp"
#include "y/system/exception.hpp"

#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <iostream>

namespace Yttrium
{

    namespace
    {
        static AtExit   axStack[AtExit::MaxNumber] = {};
        static size_t   axCount = 0;

        static inline int axCompare(const void *lhs,
                                    const void *rhs) noexcept
        {
            const AtExit &L = *static_cast<const AtExit *>(lhs);
            const AtExit &R = *static_cast<const AtExit *>(rhs);
            return R.longevity - L.longevity;
        }

        static inline void axCall(void) noexcept
        {
            while(axCount>0)
            {
                AtExit &ax = axStack[--axCount];
                assert(0!=ax.procedure);
                try
                {
                    ax.procedure(ax.arguments);
                }
                catch(...)
                {
                    std::cerr << "caugth exception during " << AtExit::CallSign << std::endl;
                }
                memset(&ax,0,sizeof(AtExit));
            }
        }

    }
    const char * const AtExit:: CallSign = "AtExit";

    void AtExit::Register(Procedure       procedure,
                          Arguments       arguments,
                          const Longevity longevity)
    {
        // sanity check
        assert(0!=procedure);


        // check capacity
        if(axCount>=AtExit::MaxNumber) throw Specific:: Exception(CallSign,"Exceeded capacity");

        // check no multiple longevities
        for(size_t i=0;i<axCount;++i)
        {
            if(longevity==axStack[i].longevity) throw Specific:: Exception(CallSign,"Multiple longevity %d", longevity);
        }

        // initialize on first insertion
        if(axCount<=0)
        {
            memset(axStack,0,sizeof(axStack));
            errno = 0;
            if(0!=atexit(axCall))
            {
                throw Libc::Exception(errno,"atexit");
            }
        }

        // create new entry
        {
            AtExit &ax   = axStack[axCount++];
            ax.procedure = procedure;
            ax.arguments = arguments;
            ax.longevity = longevity;
        }

        // sort by increasing longevity
        qsort(axStack, axCount, sizeof(AtExit),axCompare);
    }



}

