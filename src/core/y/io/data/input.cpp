
#include "y/io/data/input.hpp"
#include "y/memory/buffer/ro.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    InputDataStream:: ~InputDataStream() noexcept
    {
        Coerce(curr) = 0;
        Coerce(last) = 0;
        Coerce(base) = 0;
    }

    InputDataStream:: InputDataStream(const void *addr, const size_t size) noexcept :
    curr( static_cast<const char *>(addr) ),
    last( curr+size ),
    base( curr )
    {
        assert(Good(addr,size));
    }

    InputDataStream:: InputDataStream(const Memory::ReadOnlyBuffer &buff) noexcept :
    curr( static_cast<const char *>(buff.ro_addr()) ),
    last( curr+buff.measure() ),
    base( curr )
    {
    }

    bool InputDataStream:: query(char &c)
    {
        if(curr>=last)
        {
            return false;
        }
        else
        {
            c = *(curr++);
            return true;
        }
    }

    bool InputDataStream:: ready()
    {
        return curr<last;
    }

    const char * const InputDataStream :: CallSign = "InputDataStream";

    void InputDataStream:: store(const char c)
    {
        static const char fn[] = "store";

        if(curr<=base)  throw Specific::Exception(CallSign,"%s(already at entry)",fn);
        if(curr[-1]!=c) throw Specific::Exception(CallSign,"%s(invalid char)",fn);
        --curr;
    }

    const char * InputDataStream:: callSign() const noexcept
    {
        return CallSign;
    }

}
