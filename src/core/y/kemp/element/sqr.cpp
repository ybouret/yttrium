#include "y/kemp/element/sqr.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        const Element::UnaryAPI toto =
        {
            ComputeSquare<uint64_t,uint32_t>::Result,
            ComputeSquare<uint64_t,uint32_t>::ResTMX,
        };

        //const Element::UnaryAPI Element::Sqr[Kinds];



    }

}
