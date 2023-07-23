
#include "y/counting/counting.hpp"

namespace Yttrium
{

    Counting:: ~Counting() noexcept
    {
    }

    Counting:: Counting(const Cardinality n) noexcept :
    index(1),
    total(n)
    {
        assert(total>0);
    }

    void Counting:: boot() noexcept
    {
        doBoot();
        Coerce(index) = 1;
    }


    bool Counting:: next() noexcept
    {
        assert(index<=total);
        if(!doNext())
            return false;
        ++Coerce(index);
        return true;
    }

}
