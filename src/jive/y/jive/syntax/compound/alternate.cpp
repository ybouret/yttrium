#include "y/jive/syntax/compound/alternate.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Alternate:: ~Alternate() noexcept
            {
            }

            Alternate & Alternate:: operator |=(const Rule &rr)
            {
                (*this) << rr;
                return *this;
            }

            bool Alternate:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                
                return false;
            }
            
        }

    }

}

