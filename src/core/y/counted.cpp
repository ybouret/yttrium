
#include "y/counted.hpp"

namespace Yttrium
{

    Counted:: Counted() noexcept : nref(0) {}

    Counted:: ~Counted() noexcept
    {
        assert(0==quantity());
    }

    void Counted:: withhold() noexcept
    {
        ++nref;
    }

    bool Counted:: liberate() noexcept
    {
        assert(nref>0);
        return (--nref <= 0);
    }

    size_t Counted:: quantity() const noexcept
    {
        return nref;
    }

}


