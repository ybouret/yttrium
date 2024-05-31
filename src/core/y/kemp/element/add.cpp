
#include "y/kemp/element/add.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        const Element::BinaryProc Element:: Add[Kinds] =
        {
            Y_Kemp_Ops(ElementAdd)
        };

        const Element::BinaryProcEx Element:: AddEx[Kinds] =
        {
            Y_Kemp_Ops(ElementAddEx)
        };


    }

}
