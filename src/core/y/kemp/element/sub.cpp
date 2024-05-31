#include "y/kemp/element/sub.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        const Element::BinaryProc Element:: Sub[Kinds] =
        {
            Y_Kemp_Ops(ElementSub)
        };

        const Element::BinaryProcEx Element:: SubEx[Kinds] =
        {
            Y_Kemp_Ops(ElementSubEx)
        };
    }

}

