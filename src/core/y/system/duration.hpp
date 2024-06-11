//! \file

#ifndef Y_Duration_Included
#define Y_Duration_Included 1

#include "y/config/starting.hpp"
#include "y/calculus/align.hpp"


#if defined(Y_WIN)
#include <winsock2.h>
#endif


namespace Yttrium
{

    class Duration
    {
    public:
        enum Status
        {
            Standard=0,
            Infinite=1
        };
        static const size_t Required = 4 * sizeof(uint64_t);
        static const size_t MaxWait  = 60;

        Duration(const double s)               noexcept;
        Duration(const Duration &)             noexcept;
        Duration & operator=(const Duration &) noexcept;
        Duration & operator=(const double   s) noexcept;
        ~Duration()                            noexcept;

        Status                 status() const noexcept;
        operator double()               const noexcept;
        struct timeval *       tv()           noexcept;
        const struct timeval * tv()     const noexcept;


    private:
        mutable void *wksp[ Y_WORDS_GEQ(Required) ];
        void ldz() noexcept;
        void ld(double s) noexcept;
        Status &         getStatus() const noexcept;
        struct timeval & getTV()     const noexcept;

    };


}

#endif
