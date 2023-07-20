#include "string.hpp"

#include "y/memory/allocator/pooled.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/utils.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Core
    {
        template <typename T> static inline
        T *StringAcquire(size_t &items,
                         size_t &bytes)
        {
            static Memory::Allocator &mgr = Memory::Pooled::Instance();
            return mgr.allocate<T>(items,bytes);
        }

        template <typename T> static inline
        void StringRelease(T *    &entry,
                           size_t &items,
                           size_t &bytes) noexcept
        {
            static Memory::Allocator &mgr = Memory::Pooled::Location();
            mgr.withdraw(entry,items,bytes);
        }

        static const size_t StringMinChars = 31;
    }
}

#define Y_STRING_PROLOG() Object(), Counted(), Writable<CH>()

#define CH char
#include "string.hxx"

#undef CH
#define CH uint32_t
#include "string.hxx"

