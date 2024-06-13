

//! \file

#ifndef Y_System_HRT_Included
#define Y_System_HRT_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    class HRT
    {
    public:
        static const unsigned Minute     = 60;
        static const unsigned Hour       = 60 * Minute;
        static const unsigned Day        = 24 * Hour;
        static const unsigned MaxDays    = 99;
        static const unsigned MaxSeconds = MaxDays * Day;
        static const size_t   Length     = 4 * 2 + 3;

        ~HRT()              noexcept;
        HRT(const HRT &)    noexcept;
        HRT(double seconds) noexcept;
        Y_OSTREAM_PROTO(HRT);

        const char * operator*() const noexcept { return fmt; }

        
    private:
        Y_DISABLE_ASSIGN(HRT);
        char fmt[16];

    };
}

#endif

