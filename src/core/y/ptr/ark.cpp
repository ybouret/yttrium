#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Core
    {
        ArkPtr:: ~ArkPtr() noexcept {}
        ArkPtr::  ArkPtr() noexcept {}

        const char * const ArkPtr::CallSign = "ArkPtr";

        void ArkPtr:: NoNull(void *handle) noexcept
        {
            Ptr::Critical(handle,CallSign);
        }


    }
}
