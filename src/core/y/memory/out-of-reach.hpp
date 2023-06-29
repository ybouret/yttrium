//! \file

#ifndef Y_Memory_OutOfReach_Included
#define Y_Memory_OutOfReach_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Memory
    {
        struct OutOfReach
        {
            static void *       Addr(void *       addr)                                           noexcept;
            static const void * Addr(const void * addr)                                           noexcept;
            static void       * Zero(void *       addr,   const size_t size)                      noexcept;
            static void       * Move(void *       target, const void * source, const size_t size) noexcept;
            static void       * Mov0(void *       target, void *       source, const size_t size) noexcept;
            static void       * Swap(void *       target, void *       source, const size_t size) noexcept;
            static ptrdiff_t    Diff(const void * a,      const void * b)                         noexcept;
            static bool         Are0(const void * addr,   const size_t size)                      noexcept;
        };
    }

}

#define Y_STATIC_ZVAR(VAR) Yttrium::Memory::OutOfReach::Zero(&VAR,sizeof(VAR))
#define Y_STATIC_ZARR(ARR) Yttrium::Memory::OutOfReach::Zero(ARR, sizeof(ARR))

#endif

