
#include "y/lingo/lexical/action/call.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            Call:: ~Call() noexcept {}


            Call:: Call(const bool flag, const Caption &call) noexcept :
            Action_(UUID,flag),
            name(call)
            {
                Y_Lingo_Action(Call);
            }
            

        }

    }

}

