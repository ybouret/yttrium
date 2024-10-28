

#include "y/lingo/lexical/action/drop.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            Drop:: Drop(const bool flag) noexcept : Action_(UUID,flag)
            {
                Y_Lingo_Action(Drop);
            }


            Drop:: ~Drop() noexcept
            {
            }


        }

    }

}

