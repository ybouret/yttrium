
#include "y/lingo/syntax/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Rule:: ~Rule() noexcept
            {
            }


            void Rule:: graphViz(OutputStream &fp) const
            {
                Enter(fp, "G");
                viz(fp);
                Leave(fp);
            }
        }

    }
}
