
#include "y/jive/pattern/all.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Pattern * Pattern::IgnoreCase(Pattern *p)
        {
            assert(0!=p);
            AutoPtr<Pattern> guard(p);

            return guard.yield();
        }
    }

}
