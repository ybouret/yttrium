
#include "y/stream/hash/output.hpp"
#include "y/hashing/function.hpp"

namespace Yttrium
{

    HashingStream:: ~HashingStream() noexcept
    {
    }

    HashingStream:: HashingStream(Hashing::Function &usr) noexcept :
    OutputStream(),
    hfn(usr)
    {
    }

    void HashingStream:: write(const char c) noexcept
    {
        hfn.run(&c,1);
    }

    void HashingStream:: flush() noexcept {}

    const char * const HashingStream:: CallSign = "HashingStream";
    
    const char * HashingStream:: callSign()  const noexcept
    {
        return CallSign;
    }


}

