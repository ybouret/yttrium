#include "y/associative/little-endian-addressed.hpp"

namespace Yttrium
{
    LittleEndianAddressed:: LittleEndianAddressed() noexcept :
    leak(*this)
    {
    }

    LittleEndianAddressed:: ~LittleEndianAddressed() noexcept
    {
    }

}
