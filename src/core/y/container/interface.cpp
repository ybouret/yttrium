
#include "y/container/interface.hpp"
#include "y/type/ints.hpp"
#include "y/system/exception.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    Container:: ~Container() noexcept
    {
    }

    Container:: Container() noexcept : Collection()
    {
        
    }

    size_t Container:: available() const noexcept
    {
        assert(size()<=capacity());
        return capacity()-size();
    }


    size_t Container:: NextCapacity(const size_t n)
    {
        static const size_t MaxCapacity = IntegerFor<size_t>::Maximum;
        static const size_t MinCapacity = 16;

        if(n>=MaxCapacity) throw Specific::Exception("Container:NextCapacity","already at maximum");
        const size_t MaxIncrease = MaxCapacity-n;
        const size_t OptIncrease = NextPowerOfTwo(n>>1);
        const size_t Increase    = Min(MaxIncrease,OptIncrease);

        return Max(n+Increase,MinCapacity);
    }

    size_t Container:: NextIncrease(const size_t n)
    {
        return NextCapacity(n) - n;
    }

}
