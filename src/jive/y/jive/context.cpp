#include "y/jive/context.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Context:: ~Context() noexcept
        {

        }

        Context:: Context(const Context &ctx) noexcept :
        tag( ctx.tag ),
        line( ctx.line ),
        column( ctx.column )
        {
        }
        
    }

}

