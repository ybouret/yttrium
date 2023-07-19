#include "y/io/cache.hpp"

namespace Yttrium
{
    namespace IO
    {

        Cache:: Cache() noexcept : CxxListOf<Char>() {}
        Cache:: ~Cache() noexcept {}

        Cache & Cache:: operator<<(const uint8_t c)
        {
            pushTail( new Char(c) );
            return *this;
        }

        Cache & Cache:: operator>>(const uint8_t c)
        {
            pushHead( new Char(c) );
            return *this;
        }

        Cache:: Cache(const Cache &other) : CxxListOf<Char>(other) {}


        Cache & Cache:: operator<<(const char *msg)
        {
            if(msg)
            {
                while(true)
                {
                    const uint8_t c = static_cast<uint8_t>(*(msg++));
                    if(c<=0) break;
                    pushTail( new Char(c) );
                }
            }
            return *this;
        }

        uint8_t Cache:: pullTail() noexcept
        {
            assert(size>0);
            Char         *chr = popTail();
            const uint8_t res = **chr;
            delete chr;
            return res;
        }

        uint8_t Cache:: pullHead() noexcept
        {
            assert(size>0);
            Char         *chr = popHead();
            const uint8_t res = **chr;
            delete chr;
            return res;
        }


    }
}
