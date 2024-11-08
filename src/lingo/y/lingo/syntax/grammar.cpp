
#include "y/lingo/syntax/grammar.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() noexcept
            {
                
            }


            Grammar::ConstInterface & Grammar:: surrogate() const noexcept
            {
                return rules;
            }

        }

    }

}
