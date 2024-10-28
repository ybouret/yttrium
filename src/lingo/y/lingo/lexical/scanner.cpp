
#include "y/lingo/lexical/scanner.hpp"


namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            Scanner:: ~Scanner() noexcept
            {
            }

            Scanner::ConstInterface &   Scanner:: surrogate() const noexcept
            {
                return rules;
            }

        }

    }

}

