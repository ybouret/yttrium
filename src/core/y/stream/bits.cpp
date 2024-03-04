
#include "y/stream/bits.hpp"

namespace Yttrium
{

    namespace IO
    {
        Y_SHALLOW_IMPL(_0);
        Y_SHALLOW_IMPL(_1);

    }

    StreamBits:: StreamBits() noexcept : CxxListOf<IO::Char>(), pool() {}

    StreamBits:: ~StreamBits() noexcept
    {
    }

    void StreamBits:: free() noexcept
    {
        while(size>0) pool.store( popTail() );
    }

    IO::Char * StreamBits:: build(const uint8_t b)
    {
        if(pool.size>0)
        {
            IO::Char *ch = pool.query();
            **ch = b;
            return ch;
        }
        else
        {
            return new IO::Char(b);
        }
    }

    void StreamBits:: toTail(const uint8_t b)
    {
        pushTail( build(b) );
    }

    StreamBits & StreamBits:: operator<<( const IO::_1_ &)
    {
        toTail(_1);
        return *this;
    }

    StreamBits & StreamBits:: operator<<( const IO::_0_ &)
    {
        toTail(_0);
        return *this;
    }

    StreamBits & StreamBits:: fill()
    {
        while( 0 != (size&0x7) ) toTail(_0);
        return *this;
    }


}

#include "y/string.hpp"

namespace Yttrium
{
    std::ostream & operator<<(std::ostream &os, const StreamBits &io)
    {
        String str(io.size,AsCapacity,false);
        for(const IO::Char *ch = io.head;ch;ch=ch->next)
        {
            str += (0!=**ch) ? '1' : '0';
        }
        return os << str;
    }

}
