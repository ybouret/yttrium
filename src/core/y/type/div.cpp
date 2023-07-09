
#include "y/type/div.hpp"

namespace Yttrium
{
    namespace Core
    {
        const char * const DivInt::      CallSign = "DivInt";
        const char * const DivLong::     CallSign = "DivLong";
        const char * const DivLongLong:: CallSign = "DivLongLong";

        const DivInt:: Proc DivInt::          Call =  div;
        const DivLong::Proc DivLong::         Call = ldiv;
        const DivLongLong::Proc DivLongLong:: Call = lldiv;

    }
}
