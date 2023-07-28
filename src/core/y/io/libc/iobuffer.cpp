
#include "y/io/libc/iobuffer.hpp"
#include "y/calculus/base2.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include <cstdio>

namespace Yttrium
{

    namespace Libc
    {

        static inline char * MemAcquire(size_t &bytes, unsigned &shift)
        {
            static Memory::Dyadic &mgr = Memory::Dyadic::Instance();

            void *res = mgr.acquireBlock(shift);
            bytes = size_t(1) << shift;
            return static_cast<char *>(res);
        }


        static inline void MemRelease(char  * &blockAddr, unsigned blockShift) noexcept
        {
            static Memory::Dyadic &mgr = Memory::Dyadic::Location();
            assert(0!=blockAddr);
            mgr.releaseBlock(*(void **)&blockAddr,blockShift);
        }

        void IOBuffer:: release() noexcept
        {
            assert(0!=entry);
            MemRelease(Coerce(entry),Coerce(shift));
            Coerce(bytes) = 0;
        }

        IOBuffer:: IOBuffer() :
        IO::Chars(),
        bytes( BUFSIZ ),
        shift( Base2<size_t>::LogFor( Coerce(bytes) )  ),
        entry( MemAcquire(Coerce(bytes),Coerce(shift)) ),
        stock()
        {
            std::cerr << "acquired " << bytes << " for " << BUFSIZ << std::endl;
            try {
                stock.reserve(bytes);
            }
            catch(...) { release(); throw; }
        }

        IOBuffer:: ~IOBuffer() noexcept
        {
            release();
        }

        void IOBuffer:: ready()
        {
            while(stock.size<bytes) stock.store( new IO::Char(0) );
        }

        void IOBuffer:: prune() noexcept
        {
            assert(size+stock.size>=bytes);

            while(stock.size>0 && size+stock.size>bytes) delete stock.query();
        }

        void IOBuffer:: unget(const char c)
        {
            if(stock.size>0) {
                **pushHead( stock.query() ) = c;
            }
            else
            {
                pushHead( new IO::Char(c) );
            }
        }

    }

}
