

#include "y/chemical/type/constants.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        XVector:: ~XVector() noexcept
        {
            
        }

        XVector:: XVector() noexcept
        {

        }

        Constants:: Constants() : ArcPtr<XVector>( new XVector() ) {}

        Constants:: Constants(const Constants &other) noexcept : ArcPtr<XVector>(other) {}

        Constants:: ~Constants() noexcept
        {
        }
        
    }
}
