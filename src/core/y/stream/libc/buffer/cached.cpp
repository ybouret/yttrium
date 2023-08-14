


#include "y/stream/libc/buffer/cached.hpp"

#include <cstdio>

namespace Yttrium
{

    namespace Libc
    {

        CachedBuffer:: ~CachedBuffer() noexcept
        {
        }

        CachedBuffer:: CachedBuffer() :
        SystemBuffer(),
        IO::Chars(),
        stock()
        {
            stock.reserve(bytes);
        }


        void CachedBuffer:: ready()
        {
            while(stock.size<bytes) stock.store( new IO::Char(0) );
        }

        void CachedBuffer:: prune() noexcept
        {
            assert(size+stock.size>=bytes);
            while( (stock.size>0) && (size+stock.size>bytes) ) delete stock.query();
        }

        void CachedBuffer:: unget(const char c)
        {
            if(stock.size>0) {
                **pushHead( stock.query() ) = c;
            }
            else
            {
                pushHead( new IO::Char(c) );
            }
        }


        void CachedBuffer:: bring(const size_t lower, const size_t upper) noexcept
        {
            assert(lower<=upper);
            size_t         n = upper+1-lower; assert(stock.size>=n);  assert(n<=bytes);
            const char    *p = entry+lower;
            while(n-- > 0)
            {
                assert(stock.size>0);
                **pushTail( stock.query() ) = *(p++);
            }
        }

        char CachedBuffer:: pluck() noexcept
        {
            assert(size>0);
            IO::Char  *ch = popHead();
            const char c  = **ch;
            stock.store(ch);
            return c;
        }



    }

}


