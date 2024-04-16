
#include "y/ink/format.hpp"


namespace Yttrium
{
    namespace Ink
    {

        Format:: ~Format() noexcept
        {
        }
        

        bool Format:: matches(const char   *ext) const noexcept
        {
            const String _(ext);
            return matches(_);
        }

        const String & Format:: key() const noexcept
        {
            return name;
        }


    }

}
