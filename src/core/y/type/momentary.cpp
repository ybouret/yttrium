
#include "y/type/momentary.hpp"

namespace Yttrium
{

    Momentary:: On:: On(bool &flag) noexcept : Temporary<bool>(flag, true)
    {
    }
    

    Momentary:: On:: ~On() noexcept
    {
    }

}


namespace Yttrium
{
    Momentary:: Off:: Off(bool &flag) noexcept : Temporary<bool>(flag, false)
    {
    }

    Momentary:: Off:: ~Off() noexcept
    {
    }

}
