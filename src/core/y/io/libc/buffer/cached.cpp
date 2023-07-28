


#include "y/io/libc/buffer/cached.hpp"

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

            while(stock.size>0 && size+stock.size>bytes) delete stock.query();
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

    }

}


