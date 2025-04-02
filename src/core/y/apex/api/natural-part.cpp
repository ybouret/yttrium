
#include "y/apex/api/natural-part.hpp"

namespace Yttrium
{
    namespace Apex
    {
        const Natural & NaturalPart:: Of(const Natural &_) noexcept
        {
            return _;
        }

        const Natural & NaturalPart:: Of(const Integer &_) noexcept
        {
            return _.n;
        }
    }

}

