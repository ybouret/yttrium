
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Core
    {
        AutoPtr::  AutoPtr() noexcept {}
        AutoPtr:: ~AutoPtr() noexcept {}

        const char * const AutoPtr:: CallSign = "AutoPtr";

        void AutoPtr:: Critical(const void *addr) noexcept
        {
            Ptr::Critical(addr,CallSign);
        }
    }

}

