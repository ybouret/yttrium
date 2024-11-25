
#include "y/chemical/formula.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Formula:: ~Formula() noexcept
        {

        }

        Formula:: Formula(XNode * const node) noexcept :
        code(node)
        {
            assert(!code.isEmpty());
            walk(node,0);
        }

        Formula::ConstInterface & Formula:: surrogate() const noexcept
        {
            assert( code.isValid() );
            return *code;
        }

        void Formula:: walk(XNode * const xnode,
                            int           level)
        {
            assert(0!=xnode);

        }


    }

}


