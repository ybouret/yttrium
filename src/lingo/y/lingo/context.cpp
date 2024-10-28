
#include "y/lingo/context.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Context:: ~Context() noexcept { Coerce(column)=0; Coerce(line)=0; }

        Context:: Context(const Context &_) noexcept :
        Entity(_),
        line(_.line),
        column(_.column)
        {
        }

        Exception & Context:: stamp(Exception &excp) const noexcept
        {
            return excp.pre("%s:%lu:%lu: ", name->c_str(), line, column);
        }

        void Context:: newChar() noexcept { ++Coerce(column); }
        void Context:: newLine() noexcept { ++Coerce(line); Coerce(column)=1; }
    }

}
