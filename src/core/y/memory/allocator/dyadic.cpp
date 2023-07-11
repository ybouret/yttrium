
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/corpus.hpp"
#include "y/concurrent/memory.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"

#include <cerrno>

namespace Yttrium
{

    namespace Memory
    {

        const AtExit::Longevity Dyadic:: LifeTime = Pooled::LifeTime - 1;
        const char * const      Dyadic:: CallSign = "Memory::Dyadic";

        const char * Dyadic :: variety() const noexcept { return CallSign; }

        static Corpus *corpus = 0;
        
        Dyadic:: Dyadic() : SingletonType(), Allocator()
        {
            corpus = & Concurrent::Mem::CorpusInstance();
        }

        Dyadic:: ~Dyadic() noexcept
        {
        }

        void * Dyadic:: acquire(size_t & count, const size_t blockSize)
        {
            assert(0!=corpus);
            static Corpus &mgr = *corpus;
            const size_t   req = count * blockSize;
            if( req > Base2<size_t>::MaxPowerOfTwo )
                throw Specific::Exception(CallSign,"bytes request is too high");
            unsigned shift = 0;
            size_t   bytes = 1;
            while(bytes<req)
            {
                bytes <<= 1;
                ++shift;
            }
            Y_LOCK(Access);
            try {
                void *p = mgr[shift].acquire(); assert( OutOfReach::Are0(p,bytes) );
                count   = bytes;
                return p;
            }
            catch(...)
            {
                count = 0;
                throw;
            }
        }

        void Dyadic:: release(void *&entry, size_t &count) noexcept
        {
            assert(0!=corpus);
            assert(0!=entry);
            assert(count>0);
            assert(count<=Base2<size_t>::MaxPowerOfTwo);

            static Corpus &mgr  = *corpus;
            unsigned      shift = 0;
            size_t        bytes = 1;
            while(bytes<count)
            {
                bytes <<= 1;
                ++shift;
            }
            if(bytes!=count) Libc::CriticalError(EINVAL, "%s.release(bad count=%lu)", CallSign, (unsigned long)count );
            mgr[shift].release(entry);
            entry = 0;
            count = 0;
        }

        
    }

}
