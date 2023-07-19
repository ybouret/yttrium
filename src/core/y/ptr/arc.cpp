#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Core
    {
        ArcPtr:: ~ArcPtr() noexcept {}
        ArcPtr::  ArcPtr() noexcept {}

        const char * const ArcPtr::CallSign = "ArcPtr";

        void ArcPtr:: Critical(void *handle) noexcept
        {
            Ptr::Critical(handle,CallSign);
        }


    }
}
