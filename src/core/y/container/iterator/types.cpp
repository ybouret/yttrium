#include "y/container/iterator/types.hpp"

namespace Yttrium
{

    namespace Iterating
    {
        const char *DirectionText(const Direction d) noexcept
        {
            switch(d)
            {
                case Forward : return "Forward";
                case Reverse : return "Reverse";
            }
            return Core::Unknown;
        }

    }

}
