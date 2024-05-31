
#include "y/kemp/element/mul.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        const Element::BinaryProc Element:: MulStd[Kinds] =
        {
            Y_Kemp_Ops(ElementMulStd)
        };

        const Element::BinaryProcEx Element:: MulStdEx[Kinds] =
        {
            Y_Kemp_Ops(ElementMulStdEx)
        };

    }

}
