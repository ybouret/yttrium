#include "y/apk/natural.hpp"
#include "y/apk/component.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace APK
    {

        Natural:: ~Natural() noexcept
        {
            assert(0!=cm);
            Nullify(cm);
        }

        Natural:: Natural() :
        Number(),
        cm( new Component() )
        {
        }



    }

}


