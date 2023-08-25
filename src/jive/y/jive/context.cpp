#include "y/jive/context.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Context:: ~Context() noexcept
        {

        }

        Context:: Context(const Context &ctx) noexcept :
        tag(ctx.tag),
        line(ctx.line),
        column(ctx.column)
        {
        }

        Exception & Context::stamp(Exception &excp) const noexcept
        {
            excp.pre("%s:%u:%u: ",
                     tag->c_str(),
                     line,
                     column);
            return excp;
        }

        void Context:: newChar() noexcept
        {
            ++Coerce(column);

        }

        void Context:: newLine() noexcept
        {
            Coerce(line)++;
            Coerce(column)=1;
        }

        
    }

}

