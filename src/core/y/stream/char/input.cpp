
#include "y/stream/char/input.hpp"

namespace Yttrium
{

    const char * const InputCharStream:: CallSign = "InputCharStream";

    InputCharStream:: ~InputCharStream() noexcept
    {

    }

    InputCharStream:: InputCharStream(IO::Chars &chars) noexcept:
    InputStream(),
    cache()
    {
        cache.swapWith(chars);
    }


    const char * InputCharStream:: callSign() const noexcept
    {
        return CallSign;
    }

    bool InputCharStream:: ready() noexcept
    {
        return cache.size > 0;
    }

    bool InputCharStream:: query(char &C) noexcept
    {
        if( cache.size > 0 )
        {
            C = cache.pullHead();
            return true;
        }
        else
        {
            return false;
        }
    }


    void InputCharStream:: store(const char C)
    {
        cache.pushHead( new IO::Char(C) );
    }
}

