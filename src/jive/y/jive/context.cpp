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

#define Y_JIVE_CTX(args) tag(args), line(1), column(1)

        Context:: Context(const Tag &usr) noexcept :
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
                     (*tag)(),
                     static_cast<unsigned long>(line),
                     static_cast<unsigned long>(column));
            return excp;
        }


        
    }

}

