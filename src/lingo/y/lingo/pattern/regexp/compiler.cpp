
#include "y/lingo/pattern/regexp/compiler.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        const char * const RXC:: CallSign = "Regular Expression";

        RXC:: ~RXC() noexcept
        {
            
        }

        RXC:: RXC(const char * const       _expr,
                  const size_t             _size,
                  const Dictionary * const _dict) noexcept :
        expr(_expr),
        curr(_expr),
        last(_expr+_size),
        dict(_dict),
        depth(0)
        {
            assert(Good(_expr,_size));
            std::cerr << "expr='" << expr << "'" << std::endl;
            std::cerr << "size="  << _size << std::endl;
        }


     
    }

}

