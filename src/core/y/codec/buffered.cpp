
#include "y/codec/buffered.hpp"

namespace Yttrium
{
    BufferedCodec:: ~BufferedCodec() noexcept
    {
    }

    BufferedCodec:: BufferedCodec() noexcept : Codec(), buffer(), supply()
    {
    }


    bool BufferedCodec:: query(char &C)
    {
        if(buffer.size>0)
        {
            C = ** supply.pushTail( buffer.popHead() );
            return true;
        }
        else
        {
            return false;
        }
    }


    bool BufferedCodec:: ready() noexcept
    {
        return buffer.size > 0;
    }


    void BufferedCodec:: store(const char C)
    {
        if(supply.size>0)
        {
            **buffer.pushHead(supply.popTail()) = C;
        }
        else
        {
            buffer >> C;
        }
    }

    size_t BufferedCodec:: buffered() const noexcept
    {
        return buffer.size;
    }

    void BufferedCodec:: clearBuffer() noexcept
    {
        supply.mergeTail(buffer);
    }


}
