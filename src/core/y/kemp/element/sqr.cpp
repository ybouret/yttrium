#include "y/kemp/element/sqr.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        const Element::UnaryAPI Element::Sqr[Kinds] = Y_Kemp_UnaryAPI_Table(ComputeSquare);

    }

}
