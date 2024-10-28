#include "y/lingo/lexical/action/error.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            Error:: Error() noexcept : Action_(UUID,false)
            {
                Y_Lingo_Action(Error);
            }


            Error:: ~Error() noexcept
            {
            }


        }

    }

}

