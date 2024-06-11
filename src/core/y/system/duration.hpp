//! \file

#ifndef Y_Duration_Included
#define Y_Duration_Included 1

#include "y/config/starting.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{

    class Duration
    {
    public:
        static const size_t Required = 2 * sizeof(uint64_t);

        Duration() noexcept;
        ~Duration() noexcept;

        struct timeval * tv() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Duration);
        void *wksp[ Y_WORDS_GEQ(Required) ];

        void ldz() noexcept;
    };


}

#endif
