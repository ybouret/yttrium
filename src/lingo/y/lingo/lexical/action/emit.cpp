
#include "y/lingo/lexical/action/emit.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            Emit:: Emit(const bool flag) noexcept : Action_(UUID,flag)
            {
                Y_Lingo_Action(Emit);
            }


            Emit:: ~Emit() noexcept
            {
            }
            

        }

    }

}

