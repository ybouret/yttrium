#include "y/jive/context.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Context:: ~Context() noexcept
        {

        }

        Context:: Context(const Context &ctx) noexcept :
        mark( ctx.mark ),
        line( ctx.line ),
        column( ctx.column )
        {
        }

#define Y_JIVE_CTX(args) mark(args), line(1), column(1)

        Context:: Context(const Mark &usr) noexcept :
        Y_JIVE_CTX(usr)
        {
        }


        Context:: Context(String *str) noexcept :
        Y_JIVE_CTX(str)
        {
        }

        Exception & Context::stamp(Exception &excp) const noexcept
        {
            excp.pre("%s:%lu:%lu: ",
                     (*mark)(),
                     static_cast<unsigned long>(line),
                     static_cast<unsigned long>(column));
            return excp;
        }


        
    }

}

