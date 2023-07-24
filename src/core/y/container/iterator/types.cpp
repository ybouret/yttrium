#include "y/container/iterator/types.hpp"

namespace Yttrium
{

    namespace Iterating
    {
        const char *DirectionText(const Direction d) noexcept
        {
            switch(d)
            {
                case Forward : return "Iteraring::Forward";
                case Reverse : return "Iterating::Reverse";
            }
            return Core::Unknown;
        }

    }

}
