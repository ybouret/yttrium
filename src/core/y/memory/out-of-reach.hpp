//! \file

#ifndef Y_Memory_OutOfReach_Included
#define Y_Memory_OutOfReach_Included 1

#include "y/config/starting.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Memory
    {
        struct OutOfReach
        {
            static void *       Addr(void *       addr)                                           noexcept;
            static const void * Addr(const void * addr)                                           noexcept;
            static void *       Zero(void *       addr,   const size_t size)                      noexcept;
            static void *       Move(void *       target, const void * source, const size_t size) noexcept;
            static void *       Mov0(void *       target, void *       source, const size_t size) noexcept;
            static void *       Swap(void *       target, void *       source, const size_t size) noexcept;
            static ptrdiff_t    Diff(const void * a,      const void * b)                         noexcept;
            static bool         Are0(const void * addr,   const size_t size)                      noexcept;
            static void *       Haul(void *addr, ptrdiff_t delta) noexcept;

            template <typename T> static inline
            T *naught(T *item) noexcept
            {
                return static_cast<T*>(Zero(Destructed(item),sizeof(T)));
            }
        };
    }

}

#define Y_STATIC_ZVAR(VAR) Yttrium::Memory::OutOfReach::Zero(&VAR,sizeof(VAR))
#define Y_STATIC_ZARR(ARR) Yttrium::Memory::OutOfReach::Zero(ARR, sizeof(ARR))

#endif

