

#include "y/ptr/clone.hpp"

namespace Yttrium
{
    namespace Core
    {
        ClonePtr::  ClonePtr() noexcept {}
        ClonePtr:: ~ClonePtr() noexcept {}

        const char * const ClonePtr:: CallSign = "ClonePtr";

        void ClonePtr:: NoNull(void *handle) noexcept
        {
            Ptr::Critical(handle,CallSign);
        }


    }

}

