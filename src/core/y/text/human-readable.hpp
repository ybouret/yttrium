//! \file

#ifndef Y_Text_Plural_Included
#define Y_Text_Plural_Included 1

#include "y/config/starting.hpp"
#include "y/ios/ostream-proto.hpp"

namespace Yttrium
{
    class HumanReadable
    {
    public:
        struct Divider {
            const char     suffix;
            const uint64_t factor;
        };

        static const Divider Divide[];
        static const size_t  Number;
        


        HumanReadable(const HumanReadable &) noexcept;
        HumanReadable(const uint64_t)        noexcept;
        ~HumanReadable() noexcept;
        Y_OSTREAM_PROTO(HumanReadable);

        const unsigned quot; //!< quotient
        const unsigned rem;  //!< remainder
        const unsigned sfx;  //!< value = quot * 10^(3*sfx) + rem

    private:
        Y_DISABLE_ASSIGN(HumanReadable);
    };
}

#endif

