
#ifndef Y_Kemp_Element_Kar_Included
#define Y_Kemp_Element_Kar_Included 1

#include "y/kemp/element.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        struct Karatsuba
        {
            static Element *Mul(Element &lhs, Element &rhs, const Ops ops);
        };

    }

}

#endif

