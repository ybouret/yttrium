#include "y/io/chars.hpp"

namespace Yttrium
{
    namespace IO
    {

        Chars:: Chars() noexcept : CxxListOf<Char>() {}
        Chars:: ~Chars() noexcept {}

        Chars & Chars:: operator<<(const uint8_t c)
        {
            pushTail( new Char(c) );
            return *this;
        }

        Chars & Chars:: operator>>(const uint8_t c)
        {
            pushHead( new Char(c) );
            return *this;
        }

        Chars:: Chars(const Chars &other) : CxxListOf<Char>(other) {}


        Chars & Chars:: operator<<(const char *msg)
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

        Chars & Chars:: operator>>(const char *msg)
        {
            if(msg)
            {
                Chars sto;
                while(true)
                {
                    const uint8_t c = static_cast<uint8_t>(*(msg++));
                    if(c<=0) break;
                    sto.pushTail( new Char(c) );
                }
                mergeHead(sto);
            }
            return *this;
        }




        uint8_t Chars:: pullTail() noexcept
        {
            assert(size>0);
            Char         *chr = popTail();
            const uint8_t res = **chr;
            delete chr;
            return res;
        }

        uint8_t Chars:: pullHead() noexcept
        {
            assert(size>0);
            Char         *chr = popHead();
            const uint8_t res = **chr;
            delete chr;
            return res;
        }


    }

}

#include "y/memory/buffer/of.hpp"

namespace Yttrium
{
    namespace IO
    {
        std::ostream & operator<<(std::ostream &os, const Chars &chars)
        {
            Memory::BufferOf<char> buff(chars.size+1);
            {
                size_t i = 0;
                for(const IO::Char *ch=chars.head;ch;ch=ch->next,++i)
                    buff[i] = **ch;
            }
            return os << &buff[0];
        }

    }
}
