
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/corpus.hpp"
#include "y/concurrent/memory.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"

#include <cerrno>

namespace Yttrium
{

    namespace Memory
    {

        const char * const Dyadic:: CallSign = "Memory::Dyadic";

        const char * Dyadic :: variety() const noexcept { return CallSign; }

        static Corpus *corpus = 0;
        
        Dyadic:: Dyadic() : Singleton<Dyadic,Locking::Nucleus>(), Allocator()
        {
            corpus = & Concurrent::Mem::CorpusInstance();
        }

        Dyadic:: ~Dyadic() noexcept
        {
        }

        void * Dyadic:: acquireBlock(unsigned int &shift)
        {
            assert(0!=corpus);
            static Corpus &mgr = *corpus;
            if(shift>mgr.MaxShift) throw Specific::Exception(CallSign,"shift request is too high");

            Y_LOCK(Access);
            return mgr[shift].acquire();
        }

        void * Dyadic:: acquire(size_t & count, const size_t blockSize)
        {
            assert(0!=corpus);
            static Corpus &mgr = *corpus;
            const size_t   req = count * blockSize;
            if( req > Base2<size_t>::MaxPowerOfTwo )
                throw Specific::Exception(CallSign,"bytes request is too high");
            size_t         bytes = req;
            const unsigned shift = Base2<size_t>::LogFor(bytes);
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
            size_t         bytes = count;
            const unsigned shift = Base2<size_t>::LogFor(bytes);
            if(bytes!=count) Libc::CriticalError(EINVAL, "%s.release(bad count=%lu)", CallSign, (unsigned long)count );

            Y_LOCK(Access);
            mgr[shift].release(entry);
            entry = 0;
            count = 0;
        }

        
    }

}
