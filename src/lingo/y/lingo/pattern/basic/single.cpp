#include "y/lingo/pattern/basic/single.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Single:: ~Single() noexcept {}
        
        Single::  Single(const uint8_t _) noexcept :
        Pattern(UUID),
        byte(_)
        {
        }

        Single:: Single(const Single &_) noexcept :
        Pattern(_),
        byte(_.byte)
        {
        }

        Pattern * Single:: clone() const { return new Single( *this ); }

        size_t Single:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp)+1;
            fp.write(byte);
            return ans;
        }

    }
}
