
//! \file

#ifndef Y_Counted_Included
#define Y_Counted_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    class Counted
    {
    public:
        explicit Counted() noexcept;
        virtual ~Counted() noexcept;

        void   withhold() noexcept;
        bool   liberate() noexcept;
        size_t quantity() const noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Counted);
        size_t nref;
    };
};

#endif
