#include "string.hpp"

#include "y/memory/allocator/pooled.hpp"
#include "y/container/algo/reverse.hpp"
#include "y/memory/wad.hpp"
#include "y/type/utils.hpp"
#include "y/text/ops.hpp"
#include <cstring>

#include "y/random/bits.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"
#include "y/container/light-array.hpp"

namespace Yttrium
{
    namespace Core
    {
        StringCommon:: ~StringCommon() noexcept {}
        StringCommon::  StringCommon() noexcept {}

        size_t StringCommon:: BlocksFor(const size_t numChars) noexcept
        {
            return 1 + Max(MinChars,numChars);
        }

        template <>
        const char * String<char>:: callSign() const noexcept { return "String"; }

        template <>
        const char * String<uint32_t>:: callSign() const noexcept { return "String32"; }

        static const size_t INFO_SIZE = 256;

        static inline
        const char * MakeInfo(char * const       info,
                              const char * const name,
                              const char * const data) noexcept
        {
            assert(0!=name);
            assert(0!=data);
            memset(info,0,INFO_SIZE);
            strncpy(info,name,INFO_SIZE);
            strncat(info,data,INFO_SIZE);
            return info;
        }

    }



}

#define Y_STRING_PROLOG() \
Identifiable(),           \
Collection(),             \
StringCommon(),           \
Sequence<CH>(),           \
Writable<CH>(),           \
WritableContiguous<CH>(), \
Serializable()


#define Y_STRING_CODE(NUM_CHARS)               \
Object(),                                      \
WadType( StringCommon::BlocksFor(NUM_CHARS) ), \
data(    this->lead()        ),                \
item(    this->warp()        ),                \
size(0), maxi(capacity-1)

#define CH char
#include "string.hxx"

#undef CH
#define CH uint32_t
#include "string.hxx"

