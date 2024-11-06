
#include "y/lingo/syntax/internal/wildcard.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Wildcard:: ~Wildcard() noexcept
            {

            }

            void Wildcard:: checkRobustness() const
            {
                if( rule.flimsy() ) throw Specific::Exception( name->c_str(), "<%s> is not robust", rule.name->c_str() );
            }

        }

    }

}


