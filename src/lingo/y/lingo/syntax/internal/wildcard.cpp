
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

            const char Wildcard::Constructor[] = "constructor";

            void Wildcard:: checkRobustness(const char * const fn) const
            {
                assert(0!=fn);
                if( rule.flimsy() )
                    throw Specific::Exception( name->c_str(), "<%s> is not robust in %s", rule.name->c_str(), fn);
            }

        }

    }

}


