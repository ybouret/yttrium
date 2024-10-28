

#include "y/lingo/lexical/action/back.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            Back:: Back(const bool flag) noexcept : Action_(UUID,flag)
            {
                Y_Lingo_Action(Back);
            }


            Back:: ~Back() noexcept
            {
            }


        }

    }

}

