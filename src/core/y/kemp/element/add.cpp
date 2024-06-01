
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


        const Element::BinaryAPI toto[Element::Kinds] =
        Y_Kemp_BinaryAPI_Table(Addition);
        
#if 0
        {
            Compute<Addition,uint64_t,uint32_t>::Result,
            Compute<Addition,uint64_t,uint32_t>::ResTMX,
            Compute<Addition,uint64_t,uint32_t>::ResL64,
            Compute<Addition,uint64_t,uint32_t>::ResR64
        };
#endif

    }

}
