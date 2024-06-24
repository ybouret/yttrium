
#include "y/cipher/stream/buffered.hpp"

namespace Yttrium
{
    BufferedStreamCipher:: ~BufferedStreamCipher() noexcept
    {
    }

    BufferedStreamCipher:: BufferedStreamCipher() noexcept : StreamCipher(), buffer()
    {

    }


    bool BufferedStreamCipher:: ready() noexcept
    {
        return buffer.size > 0;
    }

    void BufferedStreamCipher:: store(const char C)
    {
        buffer >> C;
    }

    bool BufferedStreamCipher:: query(char &C)
    {
        if(buffer.size>0)
        {
            C = buffer.pullHead();
            return true;
        }
        else
        {
            return false;
        }
    }

}
